#pragma once

#include "termu/Catalogo.hpp"
#include "termu/Personagem.hpp"
#include "termu/Entrada.hpp"
#include "termu/Aleatorio.hpp"
#include "termu/Loja.hpp"

#include <chrono>
#include <memory>
#include <ostream>

namespace termu {

class Jogo {
public:
    Jogo(std::istream& entrada, std::ostream& saida);
    void executar();

private:
    bool menuTitulo();
    bool criarPersonagem();
    void hub();
    void explorar();
    void menuLoja();
    void menuInventario();
    void menuStatus();
    void menuEvolucao();
    void mostrarStatus() const;
    void mostrarInventario() const;
    const ModeloMonstro& escolherModeloMonstro();
    void fimDeJogo();

    std::ostream& saida_;
    Entrada entrada_;
    Aleatorio aleatorio_;
    Loja loja_;
    std::unique_ptr<Personagem> jogador_;
    int monstrosDerrotados_{0};
    int exploracoes_{0};
    std::chrono::steady_clock::time_point inicioSessao_;
    bool executando_{true};
};

} // namespace termu
