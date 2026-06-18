#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

const std::string reset = "\033[0m";
const std::string negrito = "\033[1m";
const std::string vermelho = "\033[91m";
const std::string verde = "\033[92m";
const std::string amarelo = "\033[93m";
const std::string azul = "\033[94m";
const std::string magenta = "\033[95m";
const std::string ciano = "\033[96m";
const std::string cinza = "\033[90m";

void configurarTerminal()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE saida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;

    if (GetConsoleMode(saida, &modo))
    {
        SetConsoleMode(saida, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

void esperar(int milissegundos)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milissegundos));
}

std::string barra(int atual, int maximo, int tamanho, const std::string& cor)
{
    const int preenchido = atual * tamanho / maximo;
    std::string resultado = cor + "[";

    for (int i = 0; i < tamanho; i++)
    {
        resultado += i < preenchido ? "█" : "░";
    }

    return resultado + "]" + reset;
}

void linha()
{
    std::cout << cinza
              << "======================================================================"
              << reset << '\n';
}

}

int main()
{
    configurarTerminal();

    std::cout << "\033[2J\033[H";

    linha();
    std::cout << amarelo << negrito
              << "                            T E R M U\n"
              << reset;
    std::cout << cinza
              << "                  RPG de turno no terminal\n";
    linha();

    std::cout << '\n'
              << ciano << negrito << "  Tadeu, o Clérigo" << reset
              << "                              "
              << vermelho << negrito << "Guardião Sombrio" << reset << '\n';

    std::cout << ciano
              << "       _____                                      .-\"\"\"\"-.\n"
              << "      /     \\                                   .'  .--.  '.\n"
              << "     |  o o  |                                 /   / >< \\   \\\n"
              << "     |   ^   |                                |    \\ /\\ /    |\n"
              << "     |  '-'  |                                |     '--'     |\n"
              << "      \\_____/                                  \\   .____.   /\n"
              << "                                                 '._\\____/_.'\n"
              << reset;

    std::cout << "\n  HP  "
              << barra(72, 90, 20, verde)
              << verde << " 72/90" << reset
              << "                 HP  "
              << barra(104, 190, 20, vermelho)
              << vermelho << " 104/190" << reset << '\n';

    std::cout << "  FÉ  "
              << barra(45, 70, 20, azul)
              << azul << " 45/70" << reset
              << "                 FASE "
              << magenta << negrito << "II - FÚRIA SOMBRIA" << reset << "\n\n";

    linha();
    std::cout << amarelo << negrito << " ÚLTIMO TURNO\n" << reset;
    std::cout << " " << vermelho << "Guardião Sombrio" << reset
              << " causou " << vermelho << negrito << "18 de dano" << reset << ".\n";
    std::cout << " " << verde << "Tadeu" << reset
              << " recuperou " << verde << negrito << "12 HP" << reset
              << " com Luz Curativa.\n";
    linha();

    std::cout << "\n" << negrito << " Sua ação:" << reset << "\n\n";
    std::cout << "  " << amarelo << "[1]" << reset << " Atacar\n";
    std::cout << "  " << amarelo << "[2]" << reset << " Usar habilidade\n";
    std::cout << "  " << amarelo << "[3]" << reset << " Usar item\n";
    std::cout << "  " << amarelo << "[4]" << reset << " Defender\n";
    std::cout << "  " << cinza << "[5] Fugir "
              << vermelho << "[INDISPONÍVEL: CHEFE]" << reset << "\n\n";

    std::cout << amarelo << "> 2" << reset << '\n';
    esperar(700);

    std::cout << "\n" << ciano << negrito
              << "Tadeu ergue o cetro e reúne sua Fé..."
              << reset << '\n';
    esperar(900);

    std::cout << amarelo << negrito
              << "CASTIGO SAGRADO!"
              << reset << '\n';
    esperar(700);

    std::cout << "O Guardião Sombrio sofreu "
              << amarelo << negrito << "42 de dano sagrado" << reset
              << " " << magenta << "[FRAQUEZA]" << reset << "\n\n";

    std::cout << "HP do chefe "
              << barra(62, 190, 30, vermelho)
              << vermelho << " 62/190" << reset << "\n\n";

    linha();
    std::cout << verde << negrito
              << " Exemplo visual concluído. Pressione Enter para fechar."
              << reset << '\n';
    linha();

    std::cin.get();
    return 0;
}
