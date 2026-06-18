#pragma once

#include "termu/Tipos.hpp"

#include <optional>
#include <string>
#include <vector>

namespace termu {

struct DefinicaoItem {
    std::string identificador;
    std::string nome;
    CategoriaItem categoria;
    int precoCompra;
    int precoVenda;
    int nivelMinimo;
    int estoque; // -1 = sem limite de estoque
    bool vendavel{true};
    bool apenasCombate{false};
    std::vector<ClassePersonagem> classesPermitidas;
    Atributos bonus;
    int quantidadeCura{0};
    int quantidadeRecurso{0};

    bool ehEquipamento() const;
    std::optional<EspacoEquipamento> espaco() const;
    bool podeSerEquipadoPor(ClassePersonagem classePersonagem) const;
};

} // namespace termu
