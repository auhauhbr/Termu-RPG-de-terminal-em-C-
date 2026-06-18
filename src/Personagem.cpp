#include "termu/Personagem.hpp"

#include "termu/Catalogo.hpp"
#include "termu/Regras.hpp"
#include "termu/Habilidade.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>

namespace termu {

Personagem::Personagem(std::string nome, ClassePersonagem classePersonagem)
    : nome_(std::move(nome)), classe_(classePersonagem) {
    desbloquearHabilidadesDisponiveis();
    recalcularAtributos();
    hpAtual_ = static_cast<int>(atributos_.hpMaximo);
    recursoAtual_ = classe_ == ClassePersonagem::Guerreiro ? 0 : static_cast<int>(atributos_.recursoMaximo);
    inventario_.adicionar("pocao_vida", 2);
}

const std::string& Personagem::nome() const { return nome_; }
ClassePersonagem Personagem::classePersonagem() const { return classe_; }
const std::string& Personagem::nomeRecurso() const { return definicaoClasse(classe_).nomeRecurso; }
int Personagem::nivel() const { return nivel_; }
std::int64_t Personagem::xp() const { return xp_; }
std::int64_t Personagem::ouro() const { return ouro_; }
int Personagem::pontosHabilidade() const { return pontosHabilidade_; }
int Personagem::hpAtual() const { return hpAtual_; }
int Personagem::recursoAtual() const { return recursoAtual_; }
const Atributos& Personagem::atributos() const { return atributos_; }
bool Personagem::estaVivo() const { return hpAtual_ > 0; }
Inventario& Personagem::inventario() { return inventario_; }
const Inventario& Personagem::inventario() const { return inventario_; }
Equipamento& Personagem::equipamento() { return equipamento_; }
const Equipamento& Personagem::equipamento() const { return equipamento_; }
const std::map<std::string, int>& Personagem::niveisHabilidades() const { return ranksHabilidades_; }
const std::vector<StatusAtivo>& Personagem::statusAtivos() const { return status_; }

void Personagem::receberDano(int quantidade) {
    hpAtual_ = std::max(0, hpAtual_ - std::max(0, quantidade));
}

void Personagem::curar(int quantidade) {
    hpAtual_ = std::min(static_cast<int>(atributos_.hpMaximo), hpAtual_ + std::max(0, quantidade));
}

bool Personagem::gastarRecurso(int quantidade) {
    if (quantidade < 0 || recursoAtual_ < quantidade) return false;
    recursoAtual_ -= quantidade;
    return true;
}

void Personagem::restaurarRecurso(int quantidade) {
    recursoAtual_ = std::min(static_cast<int>(atributos_.recursoMaximo),
                                recursoAtual_ + std::max(0, quantidade));
}

void Personagem::restaurarTudo() {
    hpAtual_ = static_cast<int>(atributos_.hpMaximo);
    recursoAtual_ = classe_ == ClassePersonagem::Guerreiro ? 0 : static_cast<int>(atributos_.recursoMaximo);
    status_.clear();
}

void Personagem::adicionarOuro(std::int64_t quantidade) { ouro_ = std::max<std::int64_t>(0, ouro_ + quantidade); }

bool Personagem::gastarOuro(std::int64_t quantidade) {
    if (quantidade < 0 || ouro_ < quantidade) return false;
    ouro_ -= quantidade;
    return true;
}

std::vector<std::string> Personagem::adicionarXp(std::int64_t quantidade) {
    std::vector<std::string> mensagens;
    xp_ += std::max<std::int64_t>(0, quantidade);
    // usei while pq pode subir mais de 1 nível com uma recompensa grande
    // cada volta libera o PH e vê se apareceu habilidade nova
    while (xp_ >= regras::xpNecessaria(nivel_)) {
        xp_ -= regras::xpNecessaria(nivel_);
        ++nivel_;
        ++pontosHabilidade_;
        recalcularAtributos();
        desbloquearHabilidadesDisponiveis();
        restaurarTudo();
        std::ostringstream mensagem;
        mensagem << "Você alcançou o nível " << nivel_ << " e recebeu 1 PH!";
        if (nivel_ % 5 == 0) {
            mensagem << " Marco de nível atingido!";
        }
        mensagens.push_back(mensagem.str());
    }
    return mensagens;
}

bool Personagem::evoluirHabilidade(const std::string& idHabilidade, std::string& mensagem) {
    auto itRank = ranksHabilidades_.find(idHabilidade);
    if (itRank == ranksHabilidades_.end()) {
        mensagem = "Essa habilidade ainda não foi desbloqueada.";
        return false;
    }
    if (itRank->second >= 5) {
        mensagem = "A habilidade já está no nivelHabilidade máximo.";
        return false;
    }
    const int custo = custoEvolucao(itRank->second);
    if (pontosHabilidade_ < custo) {
        mensagem = "Pontos de Habilidade insuficientes.";
        return false;
    }
    pontosHabilidade_ -= custo;
    ++itRank->second;
    mensagem = "Habilidade evoluída para o nivelHabilidade " + std::to_string(itRank->second) + ".";
    return true;
}

bool Personagem::equiparItem(const std::string& idItem, std::string& mensagem) {
    const DefinicaoItem* item = buscarItem(idItem);
    if (!item || !item->ehEquipamento() || inventario_.quantidadeDe(idItem) <= 0) {
        mensagem = "Item inválido ou ausente no inventário.";
        return false;
    }
    if (nivel_ < item->nivelMinimo) {
        mensagem = "Você precisa ser nível " + std::to_string(item->nivelMinimo) +
                  " para equipar " + item->nome + ".";
        return false;
    }
    if (!item->podeSerEquipadoPor(classe_)) {
        mensagem = item->nome + " não pode ser equipado pela classe " + paraTexto(classe_) + ".";
        return false;
    }

    const EspacoEquipamento espaco = *item->espaco();
    inventario_.remover(idItem);
    const std::string itemAntigo = equipamento_.equipar(espaco, idItem);
    if (!itemAntigo.empty()) inventario_.adicionar(itemAntigo);
    recalcularAtributos();
    mensagem = itemAntigo.empty() ? item->nome + " foi equipado."
                              : "Equipamento trocado por " + item->nome + ".";
    return true;
}

bool Personagem::desequiparEspaco(EspacoEquipamento espaco, std::string& mensagem) {
    const std::string idItem = equipamento_.desequipar(espaco);
    if (idItem.empty()) {
        mensagem = "Esse espaco já está vazio.";
        return false;
    }
    inventario_.adicionar(idItem);
    recalcularAtributos();
    mensagem = "Item desequipado.";
    return true;
}

bool Personagem::usarConsumivel(const std::string& idItem, bool emCombate, std::string& mensagem) {
    const DefinicaoItem* item = buscarItem(idItem);
    if (!item || item->categoria != CategoriaItem::Consumivel || inventario_.quantidadeDe(idItem) <= 0) {
        mensagem = "Você não possui esse consumível.";
        return false;
    }
    if (item->apenasCombate && !emCombate) {
        mensagem = "Este item só pode ser usado em combate.";
        return false;
    }
    inventario_.remover(idItem);
    curar(item->quantidadeCura);
    restaurarRecurso(item->quantidadeRecurso);
    mensagem = item->nome + " utilizado.";
    return true;
}

void Personagem::adicionarStatus(TipoStatus tipo, int duracao) {
    for (auto& status : status_) {
        if (status.tipo == tipo) {
            status.turnosRestantes = duracao;
            return;
        }
    }
    status_.push_back({tipo, duracao});
}

void Personagem::processarFimTurno(std::vector<std::string>& registro) {
    for (auto& status : status_) {
        if (status.tipo == TipoStatus::Queimadura) {
            const int dano = std::max(1, static_cast<int>(atributos_.hpMaximo * 0.05));
            receberDano(dano);
            registro.push_back("Queimadura causou " + std::to_string(dano) + " de dano em você.");
        } else if (status.tipo == TipoStatus::Veneno) {
            const int dano = std::max(1, static_cast<int>(atributos_.hpMaximo * 0.04));
            receberDano(dano);
            registro.push_back("Veneno causou " + std::to_string(dano) + " de dano em você.");
        }
        --status.turnosRestantes;
    }
    status_.erase(std::remove_if(status_.begin(), status_.end(),
        [](const StatusAtivo& status) { return status.turnosRestantes <= 0; }), status_.end());
}

void Personagem::regenerarNoTurno() {
    if (classe_ == ClassePersonagem::Mago) restaurarRecurso(8);
    else if (classe_ == ClassePersonagem::Ladino) restaurarRecurso(12);
    else if (classe_ == ClassePersonagem::Clerigo) restaurarRecurso(7);
}

void Personagem::recalcularAtributos() {
    const auto& definicao = definicaoClasse(classe_);
    const Atributos atributosAnteriores = atributos_;
    atributos_ = definicao.atributosBase;
    // recalcula do zero pra não ir somando bônus repetido sem querer
    // sla, parece mais seguro do que tentar desfazer cada número na mão
    const double niveisGanhos = nivel_ - 1;
    atributos_.hpMaximo += definicao.crescimentoPorNivel.hpMaximo * niveisGanhos;
    atributos_.recursoMaximo += definicao.crescimentoPorNivel.recursoMaximo * niveisGanhos;
    atributos_.ataque += definicao.crescimentoPorNivel.ataque * niveisGanhos;
    atributos_.magia += definicao.crescimentoPorNivel.magia * niveisGanhos;
    atributos_.defesa += definicao.crescimentoPorNivel.defesa * niveisGanhos;
    atributos_.resistencia += definicao.crescimentoPorNivel.resistencia * niveisGanhos;
    atributos_.velocidade += definicao.crescimentoPorNivel.velocidade * niveisGanhos;

    for (const auto& [espaco, idItem] : equipamento_.todos()) {
        (void)espaco;
        if (const DefinicaoItem* item = buscarItem(idItem)) atributos_ += item->bonus;
    }

    if (atributosAnteriores.hpMaximo > 0) hpAtual_ = std::min(hpAtual_, static_cast<int>(atributos_.hpMaximo));
    if (atributosAnteriores.recursoMaximo > 0) {
        recursoAtual_ = std::min(recursoAtual_, static_cast<int>(atributos_.recursoMaximo));
    }
}

void Personagem::desbloquearHabilidadesDisponiveis() {
    for (const auto& habilidade : habilidades()) {
        if (habilidade.classePersonagem == classe_ && habilidade.nivelMinimo <= nivel_) {
            ranksHabilidades_.try_emplace(habilidade.identificador, 1);
        }
    }
}

} // namespace termu
