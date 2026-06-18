#include "termu/Regras.hpp"

#include <algorithm>
#include <cmath>

namespace termu::regras {

int danoFisico(double ataque, double multiplicador, double defesa) {
    return std::max(1, static_cast<int>(std::floor(ataque * multiplicador - defesa * 0.5)));
}

int danoMagico(double magia, double multiplicador, double resistencia) {
    return std::max(1, static_cast<int>(std::floor(magia * multiplicador - resistencia * 0.5)));
}

int aplicarModificadoresDano(int danoBase, bool critico, double afinidade) {
    if (afinidade <= 0.0) return 0;
    const double multiplicadorCritico = critico ? 1.5 : 1.0;
    return std::max(1, static_cast<int>(std::floor(danoBase * multiplicadorCritico * afinidade)));
}

double chanceFuga(double velocidadeJogador, double velocidadeMediaInimigo) {
    return std::clamp(0.5 + (velocidadeJogador - velocidadeMediaInimigo) * 0.02, 0.1, 0.9);
}

std::int64_t xpNecessaria(int nivelAtual) {
    return static_cast<std::int64_t>(std::llround(50.0 * std::pow(nivelAtual, 1.5)));
}

std::int64_t recompensaXp(int xpBase, int nivelMonstro, int nivelJogador) {
    const int diferenca = nivelMonstro - nivelJogador;
    double fator = 1.0;
    // até 2 níveis de diferença fica xp normal
    // fora disso vai diminuindo ou aumentando sem deixar ficar absurdo
    if (diferenca > 2) fator = std::min(1.5, 1.0 + (diferenca - 2) * 0.0625);
    if (diferenca < -2) fator = std::max(0.1, 1.0 - (-diferenca - 2) * 0.1125);
    return static_cast<std::int64_t>(std::llround(xpBase * fator));
}

} // namespace termu::regras
