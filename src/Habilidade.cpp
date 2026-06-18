#include "termu/Habilidade.hpp"

#include <algorithm>
#include <cmath>

namespace termu {

int custoHabilidade(const DefinicaoHabilidade& habilidade, int nivelHabilidade) {
    const int rankSeguro = std::clamp(nivelHabilidade, 1, 5);
    const double reducao = 1.0 - ((rankSeguro - 1) * 0.05);
    const int ajustado = static_cast<int>(std::lround(habilidade.custoBase * reducao));
    return std::max(ajustado, static_cast<int>(std::lround(habilidade.custoBase * 0.5)));
}

double multiplicadorHabilidade(const DefinicaoHabilidade& habilidade, int nivelHabilidade) {
    const int rankSeguro = std::clamp(nivelHabilidade, 1, 5);
    return habilidade.multiplicador * (1.0 + ((rankSeguro - 1) * 0.15));
}

int custoEvolucao(int rankAtual) {
    return std::clamp(rankAtual, 1, 4);
}

} // namespace termu
