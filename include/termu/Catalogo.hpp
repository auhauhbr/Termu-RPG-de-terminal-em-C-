#pragma once

#include "termu/Item.hpp"
#include "termu/Habilidade.hpp"

#include <string>
#include <vector>

namespace termu {

struct DefinicaoClasse {
    ClassePersonagem tipo;
    std::string papel;
    std::string nomeRecurso;
    Atributos atributosBase;
    Atributos crescimentoPorNivel;
};

struct ModeloMonstro {
    std::string identificador;
    std::string nome;
    std::string tipo;
    int nivelBase;
    Atributos atributosBase;
    Atributos crescimentoPorNivel;
    Elemento fraqueza;
    Elemento resistencia;
    Elemento imunidade;
    PadraoIA ai;
    int xpBase;
    int ouroMinimo;
    int ouroMaximo;
    bool chefe;
};

const DefinicaoClasse& definicaoClasse(ClassePersonagem classePersonagem);
const std::vector<DefinicaoHabilidade>& habilidades();
const std::vector<DefinicaoItem>& itensCatalogo();
const std::vector<ModeloMonstro>& monstros();
const DefinicaoHabilidade* buscarHabilidade(const std::string& identificador);
const DefinicaoItem* buscarItem(const std::string& identificador);

} // namespace termu
