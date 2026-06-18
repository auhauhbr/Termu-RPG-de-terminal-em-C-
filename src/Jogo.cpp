#include "termu/Jogo.hpp"

#include "termu/Catalogo.hpp"
#include "termu/MotorCombate.hpp"
#include "termu/Regras.hpp"
#include "termu/Habilidade.hpp"

#include <algorithm>
#include <iomanip>
#include <vector>

namespace termu {

Jogo::Jogo(std::istream& entrada, std::ostream& saida)
    : saida_(saida), entrada_(entrada, saida), aleatorio_(), inicioSessao_(std::chrono::steady_clock::now()) {}

void Jogo::executar() {
    while (executando_) {
        if (!menuTitulo()) break;
    }
    saida_ << "\nObrigado por jogar Termu!\n";
}

bool Jogo::menuTitulo() {
    saida_ << "\n========================================\n"
            << "            T E R M U\n"
            << "      RPG de turno no terminal\n"
            << "========================================\n"
            << "1. Novo jogo\n2. Sair\n";
    const auto opcao = entrada_.lerInteiro("> ", 1, 2);
    if (!opcao || *opcao == 2) return false;
    if (!criarPersonagem()) return false;
    hub();
    return executando_;
}

bool Jogo::criarPersonagem() {
    saida_ << "\nEscolha sua classe:\n";
    for (int i = 0; i < 4; ++i) {
        const auto tipo = static_cast<ClassePersonagem>(i);
        const auto& definicao = definicaoClasse(tipo);
        saida_ << i + 1 << ". " << paraTexto(tipo) << " - " << definicao.papel
                << " | HP " << static_cast<int>(definicao.atributosBase.hpMaximo)
                << " | ATK " << static_cast<int>(definicao.atributosBase.ataque)
                << " | MAG " << static_cast<int>(definicao.atributosBase.magia) << '\n';
    }
    const auto escolhaClasse = entrada_.lerInteiro("> ", 1, 4);
    if (!escolhaClasse) return false;
    const auto nome = entrada_.lerNome("Nome do personagem (até 20 caracteres): ", 20);
    if (!nome) return false;

    jogador_ = std::make_unique<Personagem>(*nome, static_cast<ClassePersonagem>(*escolhaClasse - 1));
    monstrosDerrotados_ = 0;
    exploracoes_ = 0;
    inicioSessao_ = std::chrono::steady_clock::now();
    saida_ << "\n" << jogador_->nome() << ", o " << paraTexto(jogador_->classePersonagem())
            << ", inicia sua jornada com 120 Ouro e duas Poções de Vida.\n";
    return true;
}

void Jogo::hub() {
    while (executando_ && jogador_ && jogador_->estaVivo()) {
        saida_ << "\n=== HUB PRINCIPAL ===\n"
                << "1. Explorar\n2. Loja\n3. Status e habilidades\n"
                << "4. Inventário e equipamento\n5. Sair da sessão\n";
        const auto opcao = entrada_.lerInteiro("> ", 1, 5);
        if (!opcao) { executando_ = false; return; }
        if (*opcao == 1) explorar();
        else if (*opcao == 2) menuLoja();
        else if (*opcao == 3) menuStatus();
        else if (*opcao == 4) menuInventario();
        else {
            const auto confirmado = entrada_.confirmar("O progresso da sessão será perdido. Sair?");
            if (!confirmado) { executando_ = false; return; }
            if (*confirmado) { jogador_.reset(); return; }
        }
    }
}

void Jogo::explorar() {
    ++exploracoes_;
    const auto& definicao = escolherModeloMonstro();
    const int variacaoNivel = aleatorio_.inteiro(-1, 1);
    const int nivelMonstro = definicao.chefe
        ? std::max(definicao.nivelBase, jogador_->nivel())
        : std::max(1, jogador_->nivel() + variacaoNivel);
    Monstro monstro(definicao, nivelMonstro);
    saida_ << "\nUm " << monstro.nome() << " aparece!";
    if (monstro.ehChefe()) saida_ << " A presença de um CHEFE domina a área.";
    saida_ << '\n';

    MotorCombate combate(aleatorio_, entrada_, saida_);
    const ResultadoCombate resultado = combate.executar(*jogador_, monstro);
    if (resultado == ResultadoCombate::Derrota) {
        fimDeJogo();
        return;
    }
    if (resultado == ResultadoCombate::Fugiu) {
        saida_ << "Você escapou e retornou ao Hub sem recompensas.\n";
        return;
    }

    ++monstrosDerrotados_;
    const std::int64_t xp = regras::recompensaXp(monstro.xpBase(), monstro.nivel(), jogador_->nivel());
    const int ouro = aleatorio_.inteiro(monstro.ouroMinimo(), monstro.ouroMaximo());
    jogador_->adicionarOuro(ouro);
    const auto mensagensNivel = jogador_->adicionarXp(xp);
    saida_ << "\n=== VITÓRIA ===\nXP recebido: " << xp << "\nOuro recebido: " << ouro << '\n';
    for (const auto& mensagem : mensagensNivel) saida_ << mensagem << '\n';
}

void Jogo::menuLoja() {
    while (true) {
        saida_ << "\n=== LOJA === Ouro: " << jogador_->ouro()
                << "\n1. Comprar\n2. Vender\n3. Voltar\n";
        const auto opcao = entrada_.lerInteiro("> ", 1, 3);
        if (!opcao || *opcao == 3) return;

        if (*opcao == 1) {
            const auto& catalogo = itensCatalogo();
            for (std::size_t i = 0; i < catalogo.size(); ++i) {
                saida_ << i + 1 << ". " << catalogo[i].nome << " - " << catalogo[i].precoCompra
                        << " Ouro (" << paraTexto(catalogo[i].categoria) << ')';
                const int estoque = loja_.estoqueRestante(catalogo[i].identificador);
                saida_ << (estoque == -1 ? " [ilimitado]" : " [estoque: " + std::to_string(estoque) + "]")
                        << '\n';
            }
            saida_ << catalogo.size() + 1 << ". Voltar\n";
            const auto escolha = entrada_.lerInteiro("> ", 1, static_cast<int>(catalogo.size() + 1));
            if (!escolha || *escolha == static_cast<int>(catalogo.size() + 1)) continue;
            const DefinicaoItem& item = catalogo[*escolha - 1];
            int quantidade = 1;
            if (item.categoria == CategoriaItem::Consumivel) {
                const auto quantidadeInformada = entrada_.lerInteiro("Quantidade (1-99): ", 1, 99);
                if (!quantidadeInformada) return;
                quantidade = *quantidadeInformada;
            }
            std::string mensagem;
            loja_.comprar(*jogador_, item.identificador, quantidade, mensagem);
            saida_ << mensagem << '\n';
        } else {
            const auto& itens = jogador_->inventario().itens();
            if (itens.empty()) {
                saida_ << "Seu inventário está vazio.\n";
                continue;
            }
            for (std::size_t i = 0; i < itens.size(); ++i) {
                const DefinicaoItem* item = buscarItem(itens[i].idItem);
                saida_ << i + 1 << ". " << item->nome << " x" << itens[i].quantidade
                        << " - venda: " << item->precoVenda << " Ouro\n";
            }
            saida_ << itens.size() + 1 << ". Voltar\n";
            const auto escolha = entrada_.lerInteiro("> ", 1, static_cast<int>(itens.size() + 1));
            if (!escolha || *escolha == static_cast<int>(itens.size() + 1)) continue;
            const std::string idItem = itens[*escolha - 1].idItem;
            const auto quantidade = entrada_.lerInteiro("Quantidade: ", 1,
                                               jogador_->inventario().quantidadeDe(idItem));
            if (!quantidade) return;
            std::string mensagem;
            loja_.vender(*jogador_, idItem, *quantidade, mensagem);
            saida_ << mensagem << '\n';
        }
    }
}

void Jogo::menuInventario() {
    while (true) {
        mostrarInventario();
        saida_ << "\n1. Equipar item\n2. Desequipar espaco\n3. Usar consumível\n4. Voltar\n";
        const auto opcao = entrada_.lerInteiro("> ", 1, 4);
        if (!opcao || *opcao == 4) return;

        if (*opcao == 1) {
            std::vector<std::string> idsEquipamentos;
            for (const auto& entradaItem : jogador_->inventario().itens()) {
                const DefinicaoItem* item = buscarItem(entradaItem.idItem);
                if (item && item->ehEquipamento()) idsEquipamentos.push_back(item->identificador);
            }
            if (idsEquipamentos.empty()) {
                saida_ << "Nenhum equipamento disponível no inventário.\n";
                continue;
            }
            for (std::size_t i = 0; i < idsEquipamentos.size(); ++i) {
                saida_ << i + 1 << ". " << buscarItem(idsEquipamentos[i])->nome << '\n';
            }
            saida_ << idsEquipamentos.size() + 1 << ". Voltar\n";
            const auto escolha = entrada_.lerInteiro("> ", 1, static_cast<int>(idsEquipamentos.size() + 1));
            if (!escolha || *escolha == static_cast<int>(idsEquipamentos.size() + 1)) continue;
            std::string mensagem;
            jogador_->equiparItem(idsEquipamentos[*escolha - 1], mensagem);
            saida_ << mensagem << '\n';
        } else if (*opcao == 2) {
            saida_ << "1. Arma\n2. Armadura\n3. Acessório\n4. Voltar\n";
            const auto escolha = entrada_.lerInteiro("> ", 1, 4);
            if (!escolha || *escolha == 4) continue;
            std::string mensagem;
            jogador_->desequiparEspaco(static_cast<EspacoEquipamento>(*escolha - 1), mensagem);
            saida_ << mensagem << '\n';
        } else {
            std::vector<std::string> idsConsumiveis;
            for (const auto& entradaItem : jogador_->inventario().itens()) {
                const DefinicaoItem* item = buscarItem(entradaItem.idItem);
                if (item && item->categoria == CategoriaItem::Consumivel) idsConsumiveis.push_back(item->identificador);
            }
            if (idsConsumiveis.empty()) {
                saida_ << "Nenhum consumível disponível.\n";
                continue;
            }
            for (std::size_t i = 0; i < idsConsumiveis.size(); ++i) {
                saida_ << i + 1 << ". " << buscarItem(idsConsumiveis[i])->nome << '\n';
            }
            saida_ << idsConsumiveis.size() + 1 << ". Voltar\n";
            const auto escolha = entrada_.lerInteiro("> ", 1, static_cast<int>(idsConsumiveis.size() + 1));
            if (!escolha || *escolha == static_cast<int>(idsConsumiveis.size() + 1)) continue;
            std::string mensagem;
            jogador_->usarConsumivel(idsConsumiveis[*escolha - 1], false, mensagem);
            saida_ << mensagem << '\n';
        }
    }
}

void Jogo::menuStatus() {
    while (true) {
        mostrarStatus();
        saida_ << "\n1. Evoluir habilidades\n2. Voltar\n";
        const auto opcao = entrada_.lerInteiro("> ", 1, 2);
        if (!opcao || *opcao == 2) return;
        menuEvolucao();
    }
}

void Jogo::menuEvolucao() {
    std::vector<std::pair<std::string, int>> conhecidas(jogador_->niveisHabilidades().begin(),
                                                   jogador_->niveisHabilidades().end());
    if (conhecidas.empty()) return;
    while (true) {
        saida_ << "\n=== EVOLUÇÃO DE HABILIDADES === PH: " << jogador_->pontosHabilidade() << '\n';
        for (std::size_t i = 0; i < conhecidas.size(); ++i) {
            const DefinicaoHabilidade* habilidade = buscarHabilidade(conhecidas[i].first);
            const int nivelHabilidade = jogador_->niveisHabilidades().at(conhecidas[i].first);
            saida_ << i + 1 << ". " << habilidade->nome << " | Rank " << nivelHabilidade;
            if (nivelHabilidade == 5) saida_ << " (MÁX)";
            else saida_ << " | Custo: " << custoEvolucao(nivelHabilidade) << " PH";
            saida_ << '\n';
        }
        saida_ << conhecidas.size() + 1 << ". Voltar\n";
        const auto escolha = entrada_.lerInteiro("> ", 1, static_cast<int>(conhecidas.size() + 1));
        if (!escolha || *escolha == static_cast<int>(conhecidas.size() + 1)) return;
        std::string mensagem;
        jogador_->evoluirHabilidade(conhecidas[*escolha - 1].first, mensagem);
        saida_ << mensagem << '\n';
    }
}

void Jogo::mostrarStatus() const {
    const Atributos& atributos = jogador_->atributos();
    saida_ << "\n=== STATUS ===\n"
            << "Nome: " << jogador_->nome() << " | Classe: " << paraTexto(jogador_->classePersonagem())
            << " | Nível: " << jogador_->nivel() << '\n'
            << "XP: " << jogador_->xp() << '/' << regras::xpNecessaria(jogador_->nivel())
            << " | PH: " << jogador_->pontosHabilidade() << " | Ouro: " << jogador_->ouro() << '\n'
            << "HP: " << jogador_->hpAtual() << '/' << static_cast<int>(atributos.hpMaximo)
            << " | " << jogador_->nomeRecurso() << ": " << jogador_->recursoAtual()
            << '/' << static_cast<int>(atributos.recursoMaximo) << '\n'
            << "ATK " << static_cast<int>(atributos.ataque) << " | MAG " << static_cast<int>(atributos.magia)
            << " | DEF " << static_cast<int>(atributos.defesa)
            << " | RES " << static_cast<int>(atributos.resistencia)
            << " | VEL " << static_cast<int>(atributos.velocidade) << '\n'
            << "CRIT " << std::fixed << std::setprecision(0) << atributos.chanceCritico * 100
            << "% | EVA " << atributos.chanceEsquiva * 100 << "%\n"
            << "Habilidades:\n";
    for (const auto& [id, nivelHabilidade] : jogador_->niveisHabilidades()) {
        saida_ << "- " << buscarHabilidade(id)->nome << " (Rank " << nivelHabilidade << ")\n";
    }
}

void Jogo::mostrarInventario() const {
    saida_ << "\n=== INVENTÁRIO ===\nEquipados:\n";
    for (const EspacoEquipamento espaco : {EspacoEquipamento::Arma, EspacoEquipamento::Armadura,
                                     EspacoEquipamento::Acessorio}) {
        const std::string* id = jogador_->equipamento().equipado(espaco);
        saida_ << "- " << paraTexto(espaco) << ": " << (id ? buscarItem(*id)->nome : "Vazio") << '\n';
    }
    saida_ << "Mochila:\n";
    if (jogador_->inventario().itens().empty()) saida_ << "- Vazia\n";
    for (const auto& entradaItem : jogador_->inventario().itens()) {
        saida_ << "- " << buscarItem(entradaItem.idItem)->nome << " x" << entradaItem.quantidade << '\n';
    }
}

const ModeloMonstro& Jogo::escolherModeloMonstro() {
    const auto& catalogo = monstros();
    if (exploracoes_ % 5 == 0) return catalogo.back();

    std::vector<const ModeloMonstro*> candidatos;
    for (const auto& monstro : catalogo) {
        if (!monstro.chefe && monstro.nivelBase <= jogador_->nivel() + 4) candidatos.push_back(&monstro);
    }
    if (candidatos.empty()) return catalogo.front();
    return *candidatos[aleatorio_.inteiro(0, static_cast<int>(candidatos.size() - 1))];
}

void Jogo::fimDeJogo() {
    const auto tempoDecorrido = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::steady_clock::now() - inicioSessao_).count();
    saida_ << "\n========================================\n"
            << "             FIM DE JOGO\n"
            << "========================================\n"
            << "Nível alcançado: " << jogador_->nivel() << '\n'
            << "Monstros derrotados: " << monstrosDerrotados_ << '\n'
            << "Tempo de jogo: " << tempoDecorrido << " segundos\n";
    jogador_.reset();
}

} // namespace termu
