#pragma once

#include "termu/Tipos.hpp"

#include <string>

namespace termu {

struct DefinicaoHabilidade {
    std::string identificador;
    std::string nome;
    ClassePersonagem classePersonagem;
    int nivelMinimo;
    int custoBase;
    TipoDano tipoDano;
    Elemento elemento;
    double multiplicador;
    bool cura{false};
    TipoStatus status{TipoStatus::Nenhum};
    double chanceStatus{0};
    int duracaoStatus{0};
    std::string descricao;
};

int custoHabilidade(const DefinicaoHabilidade& habilidade, int nivelHabilidade);
double multiplicadorHabilidade(const DefinicaoHabilidade& habilidade, int nivelHabilidade);
int custoEvolucao(int rankAtual);

} // namespace termu
