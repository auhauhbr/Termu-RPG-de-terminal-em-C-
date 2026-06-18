#include "termu/Catalogo.hpp"
#include "termu/Personagem.hpp"
#include "termu/MotorCombate.hpp"
#include "termu/Entrada.hpp"
#include "termu/Monstro.hpp"
#include "termu/Regras.hpp"
#include "termu/Loja.hpp"
#include "termu/Habilidade.hpp"

#include <cassert>
#include <iostream>
#include <sstream>

int main() {
    using namespace termu;

    assert(regras::danoFisico(18, 1.0, 14) == 11);
    assert(regras::danoMagico(20, 1.4, 10) == 23);
    assert(regras::aplicarModificadoresDano(20, true, 1.5) == 45);
    assert(regras::aplicarModificadoresDano(20, false, 0.0) == 0);
    assert(regras::chanceFuga(100, 1) == 0.9);
    assert(regras::chanceFuga(1, 100) == 0.1);
    assert(regras::xpNecessaria(1) == 50);

    const DefinicaoHabilidade* bolaFogo = buscarHabilidade("bola_de_fogo");
    assert(bolaFogo != nullptr);
    assert(custoHabilidade(*bolaFogo, 1) == 15);
    assert(custoHabilidade(*bolaFogo, 5) == 12);
    assert(multiplicadorHabilidade(*bolaFogo, 5) > multiplicadorHabilidade(*bolaFogo, 1));

    Personagem guerreiro("Teste", ClassePersonagem::Guerreiro);
    assert(guerreiro.hpAtual() == 120);
    assert(guerreiro.niveisHabilidades().count("golpe_pesado") == 1);

    Loja loja;
    std::string mensagem;
    assert(loja.comprar(guerreiro, "espada_ferro", 1, mensagem));
    assert(guerreiro.equiparItem("espada_ferro", mensagem));
    assert(static_cast<int>(guerreiro.atributos().ataque) == 23);

    const auto& modeloChefe = monstros().back();
    Monstro chefe(modeloChefe, modeloChefe.nivelBase);
    assert(chefe.ehChefe());
    assert(chefe.afinidade(Elemento::Sombrio) == 0.0);
    assert(chefe.afinidade(Elemento::Sagrado) == 1.5);

    // simula várias escolhas "atacar"
    // a semente fixa ajuda o teste a não mudar do nada
    Personagem lutador("Arena", ClassePersonagem::Guerreiro);
    Monstro lobo(monstros().front(), 1);
    std::ostringstream acoesSimuladas;
    for (int i = 0; i < 100; ++i) acoesSimuladas << "1\n";
    std::istringstream entradaCombate(acoesSimuladas.str());
    std::ostringstream saidaCombate;
    Entrada entrada(entradaCombate, saidaCombate);
    Aleatorio aleatorio(42);
    MotorCombate combate(aleatorio, entrada, saidaCombate);
    const ResultadoCombate resultado = combate.executar(lutador, lobo);
    assert(resultado == ResultadoCombate::Vitoria || resultado == ResultadoCombate::Derrota);

    std::cout << "Todos os testes do Termu passaram.\n";
    return 0;
}
