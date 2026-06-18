#include "termu/Item.hpp"

#include <algorithm>

namespace termu {

bool DefinicaoItem::ehEquipamento() const {
    return categoria == CategoriaItem::Arma ||
           categoria == CategoriaItem::Armadura ||
           categoria == CategoriaItem::Acessorio;
}

std::optional<EspacoEquipamento> DefinicaoItem::espaco() const {
    if (categoria == CategoriaItem::Arma) return EspacoEquipamento::Arma;
    if (categoria == CategoriaItem::Armadura) return EspacoEquipamento::Armadura;
    if (categoria == CategoriaItem::Acessorio) return EspacoEquipamento::Acessorio;
    return std::nullopt;
}

bool DefinicaoItem::podeSerEquipadoPor(ClassePersonagem classePersonagem) const {
    return classesPermitidas.empty() ||
           std::find(classesPermitidas.begin(), classesPermitidas.end(), classePersonagem) != classesPermitidas.end();
}

} // namespace termu
