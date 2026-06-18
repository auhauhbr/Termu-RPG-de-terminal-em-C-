#include "termu/MotorCombate.hpp"

#include "termu/Catalogo.hpp"
#include "termu/Regras.hpp"
#include "termu/Habilidade.hpp"
#include "termu/Visual.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

namespace termu {

MotorCombate::MotorCombate(Aleatorio& aleatorio, Entrada& entrada, std::ostream& saida)
    : aleatorio_(aleatorio), entrada_(entrada), saida_(saida) {}

ResultadoCombate MotorCombate::executar(Personagem& jogador, Monstro& monstro) {
    fugiu_ = false;
    int rodada = 1;
    saida_ << '\n';
    visual::titulo(saida_, "COMBATE");
    saida_ << visual::ciano << visual::negrito << jogador.nome() << visual::reset
           << "  vs.  " << visual::vermelho << visual::negrito << monstro.nome()
           << visual::reset << '\n';

    if (monstro.ehChefe()) {
        saida_ << visual::magenta << "[CHEFE] Fugir não está disponível neste combate."
               << visual::reset << '\n';
    }

    while (jogador.estaVivo() && monstro.estaVivo() && !fugiu_) {
        saida_ << '\n' << visual::cinza << "-------------------------- "
               << visual::amarelo << "RODADA " << rodada << visual::cinza
               << " --------------------------" << visual::reset << '\n';
        jogador.regenerarNoTurno();
        mostrarEstado(jogador, monstro);

        // se empatar na velocidade o jogador vai primeiro, regra do documento
        const bool jogadorPrimeiro = jogador.atributos().velocidade >= monstro.atributos().velocidade;
        if (jogadorPrimeiro) {
            if (!turnoJogador(jogador, monstro)) return ResultadoCombate::Fugiu;
            if (monstro.estaVivo()) turnoMonstro(jogador, monstro, rodada);
        } else {
            turnoMonstro(jogador, monstro, rodada);
            if (jogador.estaVivo() && monstro.estaVivo() && !turnoJogador(jogador, monstro)) {
                return ResultadoCombate::Fugiu;
            }
        }

        std::vector<std::string> logStatus;
        if (jogador.estaVivo()) jogador.processarFimTurno(logStatus);
        if (monstro.estaVivo()) monstro.processarFimTurno(logStatus);
        for (const auto& linha : logStatus) {
            saida_ << visual::magenta << "* " << linha << visual::reset << '\n';
        }
        ++rodada;
    }
    return jogador.estaVivo() ? ResultadoCombate::Vitoria : ResultadoCombate::Derrota;
}

bool MotorCombate::turnoJogador(Personagem& jogador, Monstro& monstro) {
    while (true) {
        saida_ << '\n' << visual::negrito << "Sua ação:" << visual::reset << "\n\n";
        visual::opcao(saida_, 1, "Atacar");
        visual::opcao(saida_, 2, "Usar habilidade");
        visual::opcao(saida_, 3, "Usar item");
        visual::opcao(saida_, 4, "Defender");
        visual::opcao(saida_, 5, "Fugir", !monstro.ehChefe());
        saida_ << '\n';

        const auto escolha = entrada_.lerInteiro("> ", 1, 5);
        if (!escolha) return false;
        if (*escolha == 1) {
            ataqueBasico(jogador, monstro);
            return true;
        }
        if (*escolha == 2 && usarHabilidade(jogador, monstro)) return true;
        if (*escolha == 3 && usarItem(jogador)) return true;
        if (*escolha == 4) {
            jogadorDefendendo_ = true;
            jogador.restaurarRecurso(jogador.classePersonagem() == ClassePersonagem::Guerreiro ? 12 : 6);
            saida_ << visual::azul << jogador.nome()
                   << " assume uma postura defensiva." << visual::reset << '\n';
            return true;
        }
        if (*escolha == 5) {
            if (monstro.ehChefe()) {
                saida_ << visual::vermelho
                       << "[Erro] Não é possível fugir de um chefe."
                       << visual::reset << '\n';
                continue;
            }
            if (tentarFugir(jogador, monstro)) {
                fugiu_ = true;
                return false;
            }
            saida_ << visual::amarelo
                   << "A fuga falhou! Seu turno foi consumido."
                   << visual::reset << '\n';
            return true;
        }
    }
}

void MotorCombate::turnoMonstro(Personagem& jogador, Monstro& monstro, int rodada) {
    if (monstro.impedeAcao()) {
        saida_ << visual::magenta << monstro.nome()
               << " está atordoado e perdeu o turno." << visual::reset << '\n';
        return;
    }
    double multiplicador = 1.0;
    if (monstro.padraoIA() == PadraoIA::Agressivo) multiplicador = 1.15;
    if (monstro.padraoIA() == PadraoIA::Misto && aleatorio_.chance(0.40)) multiplicador = 1.30;
    if (monstro.padraoIA() == PadraoIA::Tanque && rodada % 3 == 0) multiplicador = 0.75;
    if (monstro.padraoIA() == PadraoIA::FasesChefe &&
        monstro.hpAtual() < monstro.atributos().hpMaximo * 0.5) {
        // abaixo de 50% o chefe fica mais bravo
        // não guardei a fase separada pq nesse projeto ele não se cura
        multiplicador = 1.55;
        saida_ << visual::magenta << visual::negrito << monstro.nome()
               << " entra em uma fase furiosa!" << visual::reset << '\n';
    }

    if (aleatorio_.chance(jogador.atributos().chanceEsquiva)) {
        saida_ << visual::ciano << jogador.nome()
               << " esquivou do ataque de " << monstro.nome() << "!"
               << visual::reset << '\n';
        jogadorDefendendo_ = false;
        return;
    }
    const bool critico = aleatorio_.chance(monstro.atributos().chanceCritico);
    int dano = regras::danoFisico(monstro.atributos().ataque, multiplicador, jogador.atributos().defesa);
    dano = regras::aplicarModificadoresDano(dano, critico, 1.0);
    if (jogadorDefendendo_) dano = std::max(1, dano / 2);
    jogador.receberDano(dano);
    if (jogador.classePersonagem() == ClassePersonagem::Guerreiro) jogador.restaurarRecurso(std::max(4, dano / 4));
    saida_ << visual::vermelho << monstro.nome() << " causou "
           << visual::negrito << dano << " de dano"
           << (critico ? " crítico" : "") << visual::reset << ".\n";
    jogadorDefendendo_ = false;
}

void MotorCombate::ataqueBasico(Personagem& jogador, Monstro& monstro) {
    if (aleatorio_.chance(monstro.atributos().chanceEsquiva)) {
        saida_ << visual::amarelo << monstro.nome()
               << " esquivou do ataque." << visual::reset << '\n';
        return;
    }
    // testa esquiva antes do crítico, pq se errou não tem dano nem status
    const bool critico = aleatorio_.chance(jogador.atributos().chanceCritico);
    int dano = regras::danoFisico(jogador.atributos().ataque, 1.0, monstro.atributos().defesa);
    dano = regras::aplicarModificadoresDano(dano, critico, monstro.afinidade(Elemento::Fisico));
    monstro.receberDano(dano);
    if (jogador.classePersonagem() == ClassePersonagem::Guerreiro) jogador.restaurarRecurso(8);
    saida_ << visual::verde << "Você causou " << visual::negrito
           << dano << " de dano" << (critico ? " crítico" : "")
           << visual::reset << ".\n";
}

bool MotorCombate::usarHabilidade(Personagem& jogador, Monstro& monstro) {
    std::vector<std::pair<const DefinicaoHabilidade*, int>> disponiveis;
    for (const auto& [id, nivelHabilidade] : jogador.niveisHabilidades()) {
        if (const DefinicaoHabilidade* habilidade = buscarHabilidade(id)) disponiveis.push_back({habilidade, nivelHabilidade});
    }
    if (disponiveis.empty()) {
        saida_ << "Nenhuma habilidade desbloqueada.\n";
        return false;
    }

    saida_ << '\n' << visual::ciano << visual::negrito
           << "Habilidades:" << visual::reset << '\n';
    for (std::size_t i = 0; i < disponiveis.size(); ++i) {
        const auto& [habilidade, nivelHabilidade] = disponiveis[i];
        const int custo = custoHabilidade(*habilidade, nivelHabilidade);
        saida_ << visual::amarelo << '[' << i + 1 << "] " << visual::reset
                << visual::corElemento(habilidade->elemento) << habilidade->nome
                << visual::reset << " (Rank " << nivelHabilidade << ", "
                << custo << " " << jogador.nomeRecurso() << ')';
        if (jogador.recursoAtual() < custo) {
            saida_ << visual::vermelho << " [RECURSO INSUFICIENTE]" << visual::reset;
        }
        saida_ << '\n';
    }
    saida_ << disponiveis.size() + 1 << ". Voltar\n";
    const auto escolha = entrada_.lerInteiro("> ", 1, static_cast<int>(disponiveis.size() + 1));
    if (!escolha || *escolha == static_cast<int>(disponiveis.size() + 1)) return false;

    const auto& [habilidade, nivelHabilidade] = disponiveis[*escolha - 1];
    const int custo = custoHabilidade(*habilidade, nivelHabilidade);
    if (!jogador.gastarRecurso(custo)) {
        saida_ << "[Erro] Recurso insuficiente para usar " << habilidade->nome << ".\n";
        return false;
    }

    if (habilidade->cura) {
        const int quantidade = std::max(1, static_cast<int>(jogador.atributos().magia *
                                                       multiplicadorHabilidade(*habilidade, nivelHabilidade)));
        jogador.curar(quantidade);
        saida_ << visual::verde << habilidade->nome << " restaurou "
               << visual::negrito << quantidade << " HP."
               << visual::reset << '\n';
        return true;
    }

    if (aleatorio_.chance(monstro.atributos().chanceEsquiva)) {
        saida_ << visual::amarelo << monstro.nome()
               << " esquivou. Nenhum status foi aplicado."
               << visual::reset << '\n';
        return true;
    }
    const bool critico = aleatorio_.chance(jogador.atributos().chanceCritico);
    int dano = habilidade->tipoDano == TipoDano::Fisico
        ? regras::danoFisico(jogador.atributos().ataque, multiplicadorHabilidade(*habilidade, nivelHabilidade),
                                monstro.atributos().defesa)
        : regras::danoMagico(jogador.atributos().magia, multiplicadorHabilidade(*habilidade, nivelHabilidade),
                               monstro.atributos().resistencia);
    dano = regras::aplicarModificadoresDano(dano, critico, monstro.afinidade(habilidade->elemento));
    monstro.receberDano(dano);
    saida_ << visual::corElemento(habilidade->elemento) << habilidade->nome
           << " causou " << visual::negrito << dano << " de dano"
           << (critico ? " crítico" : "") << visual::reset
           << " (" << paraTexto(habilidade->elemento) << ").\n";
    if (habilidade->status != TipoStatus::Nenhum && dano > 0 && aleatorio_.chance(habilidade->chanceStatus)) {
        monstro.adicionarStatus(habilidade->status, habilidade->duracaoStatus);
        saida_ << visual::magenta << monstro.nome() << " recebeu "
               << paraTexto(habilidade->status) << "." << visual::reset << '\n';
    }
    return true;
}

bool MotorCombate::usarItem(Personagem& jogador) {
    std::vector<const DefinicaoItem*> consumiveis;
    for (const auto& entradaItem : jogador.inventario().itens()) {
        if (const DefinicaoItem* item = buscarItem(entradaItem.idItem);
            item && item->categoria == CategoriaItem::Consumivel) {
            consumiveis.push_back(item);
        }
    }
    if (consumiveis.empty()) {
        saida_ << "Você não possui consumíveis.\n";
        return false;
    }
    for (std::size_t i = 0; i < consumiveis.size(); ++i) {
        saida_ << i + 1 << ". " << consumiveis[i]->nome << " x"
                << jogador.inventario().quantidadeDe(consumiveis[i]->identificador) << '\n';
    }
    saida_ << consumiveis.size() + 1 << ". Voltar\n";
    const auto escolha = entrada_.lerInteiro("> ", 1, static_cast<int>(consumiveis.size() + 1));
    if (!escolha || *escolha == static_cast<int>(consumiveis.size() + 1)) return false;
    std::string mensagem;
    const bool used = jogador.usarConsumivel(consumiveis[*escolha - 1]->identificador, true, mensagem);
    saida_ << mensagem << '\n';
    return used;
}

bool MotorCombate::tentarFugir(const Personagem& jogador, const Monstro& monstro) {
    return aleatorio_.chance(regras::chanceFuga(jogador.atributos().velocidade, monstro.atributos().velocidade));
}

void MotorCombate::mostrarEstado(const Personagem& jogador, const Monstro& monstro) const {
    const int hpJogadorMaximo = static_cast<int>(jogador.atributos().hpMaximo);
    const int hpMonstroMaximo = static_cast<int>(monstro.atributos().hpMaximo);
    const int recursoMaximo = static_cast<int>(jogador.atributos().recursoMaximo);

    saida_ << visual::ciano << visual::negrito << jogador.nome() << visual::reset
           << " | HP "
           << visual::barra(jogador.hpAtual(), hpJogadorMaximo, 20,
                            visual::corVida(jogador.hpAtual(), hpJogadorMaximo))
           << ' ' << jogador.hpAtual() << '/' << hpJogadorMaximo << '\n';
    saida_ << "  " << jogador.nomeRecurso() << ' '
           << visual::barra(jogador.recursoAtual(), recursoMaximo, 20, visual::azul)
           << ' ' << jogador.recursoAtual() << '/' << recursoMaximo << '\n';

    saida_ << visual::vermelho << visual::negrito << monstro.nome() << visual::reset
           << " | HP "
           << visual::barra(monstro.hpAtual(), hpMonstroMaximo, 20,
                            visual::corVida(monstro.hpAtual(), hpMonstroMaximo))
           << ' ' << monstro.hpAtual() << '/' << hpMonstroMaximo
           << " | Nv. " << monstro.nivel() << '\n';

    if (monstro.ehChefe() && monstro.hpAtual() < hpMonstroMaximo * 0.5) {
        saida_ << visual::magenta << visual::negrito
               << "  FASE II - FÚRIA SOMBRIA" << visual::reset << '\n';
    }
}

} // namespace termu
