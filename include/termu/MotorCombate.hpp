#pragma once

#include "termu/Personagem.hpp"
#include "termu/Entrada.hpp"
#include "termu/Monstro.hpp"
#include "termu/Aleatorio.hpp"

#include <ostream>

namespace termu {

class MotorCombate {
public:
    MotorCombate(Aleatorio& aleatorio, Entrada& entrada, std::ostream& saida);
    ResultadoCombate executar(Personagem& jogador, Monstro& monstro);

private:
    void mostrarVida(const Combatente& combatente) const;
    bool turnoJogador(Personagem& jogador, Monstro& monstro);
    void turnoMonstro(Personagem& jogador, Monstro& monstro, int rodada);
    void ataqueBasico(Personagem& jogador, Monstro& monstro);
    bool usarHabilidade(Personagem& jogador, Monstro& monstro);
    bool usarItem(Personagem& jogador);
    bool tentarFugir(const Personagem& jogador, const Monstro& monstro);
    void mostrarEstado(const Personagem& jogador, const Monstro& monstro) const;

    Aleatorio& aleatorio_;
    Entrada& entrada_;
    std::ostream& saida_;
    bool fugiu_{false};
    bool jogadorDefendendo_{false};
};

} // namespace termu
