#include "termu/Catalogo.hpp"

#include <stdexcept>

namespace termu {
namespace {

const std::vector<DefinicaoClasse> classesCadastradas{
    {ClassePersonagem::Guerreiro, "Tanque e dano físico", "Fúria",
     {120, 50, 18, 4, 14, 6, 8, 0.05, 0.05},
     {14, 4, 3, 0.5, 2.5, 1, 1, 0, 0}},
    {ClassePersonagem::Mago, "Dano mágico elemental", "Mana",
     {70, 80, 5, 20, 5, 12, 9, 0.05, 0.08},
     {7, 8, 0.5, 3.5, 1, 2, 1.2, 0, 0}},
    {ClassePersonagem::Ladino, "Críticos, esquiva e combos", "Energia",
     {85, 60, 16, 6, 8, 8, 16, 0.20, 0.18},
     {9, 5, 2.5, 1, 1.5, 1.2, 2, 0, 0}},
    {ClassePersonagem::Clerigo, "Cura e sustentação", "Fé",
     {90, 70, 8, 14, 9, 14, 10, 0.05, 0.08},
     {10, 6, 1, 2.5, 1.5, 2, 1.2, 0, 0}},
};

const std::vector<DefinicaoHabilidade> habilidadesCadastradas{
    {"golpe_pesado", "Golpe Pesado", ClassePersonagem::Guerreiro, 1, 20, TipoDano::Fisico,
     Elemento::Fisico, 1.50, false, TipoStatus::Nenhum, 0, 0, "Ataque físico de alto impacto."},
    {"lamina_giratoria", "Lâmina Giratória", ClassePersonagem::Guerreiro, 6, 45, TipoDano::Fisico,
     Elemento::Fisico, 1.80, false, TipoStatus::Nenhum, 0, 0, "Golpe amplo e devastador."},
    {"bola_de_fogo", "Bola de Fogo", ClassePersonagem::Mago, 1, 15, TipoDano::Magico,
     Elemento::Fogo, 1.40, false, TipoStatus::Queimadura, 0.30, 3, "Pode aplicar Queimadura."},
    {"lanca_de_gelo", "Lança de Gelo", ClassePersonagem::Mago, 3, 20, TipoDano::Magico,
     Elemento::Gelo, 1.20, false, TipoStatus::Lentidao, 0.40, 3, "Pode reduzir a velocidade."},
    {"ataque_furtivo", "Ataque Furtivo", ClassePersonagem::Ladino, 1, 10, TipoDano::Fisico,
     Elemento::Fisico, 1.30, false, TipoStatus::Nenhum, 0, 0, "Ataque rápido com ótima sinergia crítica."},
    {"lamina_envenenada", "Lâmina Envenenada", ClassePersonagem::Ladino, 3, 15, TipoDano::Fisico,
     Elemento::Fisico, 0.90, false, TipoStatus::Veneno, 1.0, 4, "Aplica Veneno garantido."},
    {"luz_curativa", "Luz Curativa", ClassePersonagem::Clerigo, 1, 15, TipoDano::Magico,
     Elemento::Sagrado, 1.20, true, TipoStatus::Nenhum, 0, 0, "Restaura a própria vida."},
    {"castigo_sagrado", "Castigo Sagrado", ClassePersonagem::Clerigo, 5, 25, TipoDano::Magico,
     Elemento::Sagrado, 1.10, false, TipoStatus::Nenhum, 0, 0, "Causa dano sagrado."},
};

const std::vector<DefinicaoItem> itensCadastrados{
    {"pocao_vida", "Poção de Vida", CategoriaItem::Consumivel, 30, 15, 1, -1, true, false,
     {}, {}, 45, 0},
    {"tonico_recurso", "Tônico de Recurso", CategoriaItem::Consumivel, 35, 17, 1, -1, true, false,
     {}, {}, 0, 35},
    {"espada_ferro", "Espada de Ferro", CategoriaItem::Arma, 80, 40, 1, 2, true, false,
     {ClassePersonagem::Guerreiro, ClassePersonagem::Ladino}, {0, 0, 5, 0, 0, 0, 0, 0, 0}},
    {"cajado_carvalho", "Cajado de Carvalho", CategoriaItem::Arma, 80, 40, 1, 2, true, false,
     {ClassePersonagem::Mago, ClassePersonagem::Clerigo}, {0, 8, 0, 5, 0, 0, 0, 0, 0}},
    {"armadura_couro", "Armadura de Couro", CategoriaItem::Armadura, 90, 45, 1, 2, true, false,
     {}, {18, 0, 0, 0, 4, 2, 0, 0, 0}},
    {"anel_prata", "Anel de Prata", CategoriaItem::Acessorio, 110, 55, 2, 1, true, false,
     {}, {0, 0, 2, 2, 1, 1, 1, 0.03, 0.02}},
};

const std::vector<ModeloMonstro> monstrosCadastrados{
    {"lobo", "Lobo Selvagem", "Bestial", 1,
     {55, 0, 12, 2, 5, 3, 11, 0.05, 0.05},
     {9, 0, 2, 0.2, 1, 0.5, 1, 0, 0},
     Elemento::Fogo, Elemento::Nenhum, Elemento::Nenhum, PadraoIA::Agressivo, 45, 18, 30, false},
    {"slime_gelo", "Slime de Gelo", "Elemental", 3,
     {70, 0, 8, 13, 8, 10, 6, 0.03, 0.02},
     {10, 0, 1, 2, 1, 1.5, 0.5, 0, 0},
     Elemento::Fogo, Elemento::Nenhum, Elemento::Gelo, PadraoIA::Defensivo, 60, 22, 38, false},
    {"bandido", "Bandido", "Humanoide", 5,
     {85, 0, 16, 4, 9, 6, 13, 0.12, 0.10},
     {11, 0, 2.3, 0.5, 1.2, 0.8, 1.2, 0, 0},
     Elemento::Nenhum, Elemento::Nenhum, Elemento::Nenhum, PadraoIA::Misto, 80, 30, 50, false},
    {"golem_pedra", "Golem de Pedra", "Elemental", 8,
     {145, 0, 20, 3, 18, 10, 5, 0.02, 0.01},
     {16, 0, 2.8, 0.3, 2.2, 1.2, 0.3, 0, 0},
     Elemento::Raio, Elemento::Fisico, Elemento::Nenhum, PadraoIA::Tanque, 120, 45, 70, false},
    {"guardiao_sombrio", "Guardião Sombrio", "Sombrio", 5,
     {190, 0, 22, 18, 15, 15, 10, 0.08, 0.04},
     {18, 0, 3, 2.5, 2, 2, 0.7, 0, 0},
     Elemento::Sagrado, Elemento::Fisico, Elemento::Sombrio, PadraoIA::FasesChefe, 250, 100, 160, true},
};

} // namespace

const DefinicaoClasse& definicaoClasse(ClassePersonagem classePersonagem) {
    for (const auto& definicao : classesCadastradas) {
        if (definicao.tipo == classePersonagem) return definicao;
    }
    throw std::invalid_argument("Classe de personagem desconhecida.");
}

const std::vector<DefinicaoHabilidade>& habilidades() { return habilidadesCadastradas; }
const std::vector<DefinicaoItem>& itensCatalogo() { return itensCadastrados; }
const std::vector<ModeloMonstro>& monstros() { return monstrosCadastrados; }

const DefinicaoHabilidade* buscarHabilidade(const std::string& identificador) {
    for (const auto& habilidade : habilidadesCadastradas) {
        if (habilidade.identificador == identificador) return &habilidade;
    }
    return nullptr;
}

const DefinicaoItem* buscarItem(const std::string& identificador) {
    for (const auto& item : itensCadastrados) {
        if (item.identificador == identificador) return &item;
    }
    return nullptr;
}

} // namespace termu
