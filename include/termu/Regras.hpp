#pragma once

#include <cstdint>

namespace termu::regras {

int danoFisico(double ataque, double multiplicador, double defesa);
int danoMagico(double magia, double multiplicador, double resistencia);
int aplicarModificadoresDano(int danoBase, bool critico, double afinidade);
double chanceFuga(double velocidadeJogador, double velocidadeMediaInimigo);
std::int64_t xpNecessaria(int nivelAtual);
std::int64_t recompensaXp(int xpBase, int nivelMonstro, int nivelJogador);

} // namespace termu::regras
