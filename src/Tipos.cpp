#include "termu/Tipos.hpp"

namespace termu {

Atributos& Atributos::operator+=(const Atributos& outro) {
    hpMaximo += outro.hpMaximo;
    recursoMaximo += outro.recursoMaximo;
    ataque += outro.ataque;
    magia += outro.magia;
    defesa += outro.defesa;
    resistencia += outro.resistencia;
    velocidade += outro.velocidade;
    chanceCritico += outro.chanceCritico;
    chanceEsquiva += outro.chanceEsquiva;
    return *this;
}

std::string paraTexto(ClassePersonagem valor) {
    switch (valor) {
    case ClassePersonagem::Guerreiro: return "Guerreiro";
    case ClassePersonagem::Mago: return "Mago";
    case ClassePersonagem::Ladino: return "Ladino";
    case ClassePersonagem::Clerigo: return "Clérigo";
    }
    return "Desconhecida";
}

std::string paraTexto(Elemento valor) {
    switch (valor) {
    case Elemento::Fisico: return "Físico";
    case Elemento::Fogo: return "Fogo";
    case Elemento::Gelo: return "Gelo";
    case Elemento::Raio: return "Raio";
    case Elemento::Sagrado: return "Sagrado";
    case Elemento::Sombrio: return "Sombrio";
    case Elemento::Nenhum: return "Nenhum";
    }
    return "Nenhum";
}

std::string paraTexto(CategoriaItem valor) {
    switch (valor) {
    case CategoriaItem::Arma: return "Arma";
    case CategoriaItem::Armadura: return "Armadura";
    case CategoriaItem::Acessorio: return "Acessório";
    case CategoriaItem::Consumivel: return "Consumível";
    case CategoriaItem::Material: return "Material";
    }
    return "Desconhecida";
}

std::string paraTexto(EspacoEquipamento valor) {
    switch (valor) {
    case EspacoEquipamento::Arma: return "Arma";
    case EspacoEquipamento::Armadura: return "Armadura";
    case EspacoEquipamento::Acessorio: return "Acessório";
    }
    return "Slot";
}

std::string paraTexto(TipoStatus valor) {
    switch (valor) {
    case TipoStatus::Queimadura: return "Queimadura";
    case TipoStatus::Veneno: return "Veneno";
    case TipoStatus::Lentidao: return "Lentidão";
    case TipoStatus::Atordoamento: return "Atordoamento";
    case TipoStatus::Nenhum: return "Nenhum";
    }
    return "Nenhum";
}

} // namespace termu
