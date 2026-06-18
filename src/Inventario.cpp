#include "termu/Inventario.hpp"

#include <algorithm>

namespace termu {

void Inventario::adicionar(const std::string& idItem, int quantidade) {
    if (quantidade <= 0) return;
    for (auto& entradaItem : itens_) {
        if (entradaItem.idItem == idItem) {
            entradaItem.quantidade += quantidade;
            return;
        }
    }
    itens_.push_back({idItem, quantidade});
}

bool Inventario::remover(const std::string& idItem, int quantidade) {
    if (quantidade <= 0) return false;
    auto it = std::find_if(itens_.begin(), itens_.end(),
                           [&](const EntradaInventario& entradaItem) { return entradaItem.idItem == idItem; });
    if (it == itens_.end() || it->quantidade < quantidade) return false;
    it->quantidade -= quantidade;
    if (it->quantidade == 0) itens_.erase(it);
    return true;
}

int Inventario::quantidadeDe(const std::string& idItem) const {
    for (const auto& entradaItem : itens_) if (entradaItem.idItem == idItem) return entradaItem.quantidade;
    return 0;
}

const std::vector<EntradaInventario>& Inventario::itens() const { return itens_; }

const std::string* Equipamento::equipado(EspacoEquipamento espaco) const {
    const auto it = slots_.find(espaco);
    return it == slots_.end() ? nullptr : &it->second;
}

std::string Equipamento::equipar(EspacoEquipamento espaco, const std::string& idItem) {
    std::string anterior;
    const auto it = slots_.find(espaco);
    if (it != slots_.end()) anterior = it->second;
    slots_[espaco] = idItem;
    return anterior;
}

std::string Equipamento::desequipar(EspacoEquipamento espaco) {
    const auto it = slots_.find(espaco);
    if (it == slots_.end()) return {};
    const std::string anterior = it->second;
    slots_.erase(it);
    return anterior;
}

const std::map<EspacoEquipamento, std::string>& Equipamento::todos() const { return slots_; }

} // namespace termu
