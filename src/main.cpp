#include "termu/Console.hpp"
#include "termu/Jogo.hpp"

#include <exception>
#include <iostream>

int main() {
    try {
        termu::configurarConsole();
        termu::Jogo jogo(std::cin, std::cout);
        jogo.executar();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Erro inesperado: " << error.what() << '\n';
        return 1;
    }
}
