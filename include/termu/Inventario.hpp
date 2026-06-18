#pragma once

#include "termu/Tipos.hpp"

#include <map>
#include <string>
#include <vector>

namespace termu {

struct EntradaInventario {
    std::string idItem;
    int quantidade;
};

class Inventario {
public:
    void adicionar(const std::string& idItem, int quantidade = 1);
    bool remover(const std::string& idItem, int quantidade = 1);
    int quantidadeDe(const std::string& idItem) const;
    const std::vector<EntradaInventario>& itens() const;

private:
    std::vector<EntradaInventario> itens_;
};

class Equipamento {
public:
    const std::string* equipado(EspacoEquipamento espaco) const;
    std::string equipar(EspacoEquipamento espaco, const std::string& idItem);
    std::string desequipar(EspacoEquipamento espaco);
    const std::map<EspacoEquipamento, std::string>& todos() const;

private:
    std::map<EspacoEquipamento, std::string> slots_;
};

} // namespace termu
