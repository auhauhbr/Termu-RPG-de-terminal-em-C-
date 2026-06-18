#include "termu/Visual.hpp"

#include <algorithm>

namespace termu::visual {

void limpar(std::ostream& saida)
{
    saida << "\033[2J\033[H";
}

void linha(std::ostream& saida, int tamanho)
{
    saida << cinza << std::string(tamanho, '=') << reset << '\n';
}

void titulo(std::ostream& saida, const std::string& texto)
{
    linha(saida);
    saida << amarelo << negrito << ' ' << texto << reset << '\n';
    linha(saida);
}

void opcao(std::ostream& saida, int numero, const std::string& texto,
           bool disponivel)
{
    if (disponivel)
    {
        saida << "  " << amarelo << '[' << numero << ']' << reset
              << ' ' << texto << '\n';
        return;
    }

    saida << "  " << cinza << '[' << numero << "] " << texto
          << " [INDISPONÍVEL]" << reset << '\n';
}

std::string barra(int atual, int maximo, int tamanho,
                  const std::string& cor)
{
    if (maximo <= 0) maximo = 1;

    atual = std::clamp(atual, 0, maximo);
    const int preenchido = atual * tamanho / maximo;
    std::string resultado = cor + "[";

    for (int i = 0; i < tamanho; i++)
    {
        resultado += i < preenchido ? "█" : "░";
    }

    return resultado + "]" + reset;
}

std::string corVida(int atual, int maximo)
{
    if (maximo <= 0) return vermelho;

    const double porcentagem = static_cast<double>(atual) / maximo;
    if (porcentagem > 0.6) return verde;
    if (porcentagem > 0.3) return amarelo;
    return vermelho;
}

std::string corElemento(Elemento elemento)
{
    switch (elemento)
    {
        case Elemento::Fogo: return vermelho;
        case Elemento::Gelo: return ciano;
        case Elemento::Raio: return amarelo;
        case Elemento::Sagrado: return amarelo;
        case Elemento::Sombrio: return magenta;
        case Elemento::Fisico: return cinza;
        case Elemento::Nenhum: return reset;
    }

    return reset;
}

std::vector<std::string> arteClasse(ClassePersonagem classePersonagem)
{
    switch (classePersonagem)
    {
        case ClassePersonagem::Guerreiro:
            return {
                "       /\\",
                "   ___/  \\___",
                "  /  _    _  \\",
                " |  |o|  |o|  |",
                " |      ^     |",
                "  \\  \\____/  /",
                "   '--------'"
            };

        case ClassePersonagem::Mago:
            return {
                "        /\\",
                "       /  \\",
                "      /____\\",
                "     / .--. \\",
                "    |  o  o  |",
                "    |   --   |",
                "     \\______/"
            };

        case ClassePersonagem::Ladino:
            return {
                "      .------.",
                "     /  ____  \\",
                "    |  /    \\  |",
                "    | | >  < | |",
                "    | |  --  | |",
                "     \\ \\____/ /",
                "      '------'"
            };

        case ClassePersonagem::Clerigo:
            return {
                "       _____",
                "      /  +  \\",
                "     |  o o  |",
                "     |   ^   |",
                "     |  '-'  |",
                "      \\_____/",
                "        | |"
            };
    }

    return {};
}

std::vector<std::string> arteMonstro(const std::string& nome)
{
    if (nome == "Guardião Sombrio")
    {
        return {
            "       .-\"\"\"\"-.",
            "     .'  .--.  '.",
            "    /   / >< \\   \\",
            "   |    \\ /\\ /    |",
            "   |     '--'     |",
            "    \\   .____.   /",
            "     '._\\____/_.'"
        };
    }

    if (nome == "Lobo Selvagem")
    {
        return {
            "      /\\     /\\",
            "     /  \\___/  \\",
            "    |  o     o  |",
            "    |     ^     |",
            "     \\  \\___/  /",
            "      '-------'"
        };
    }

    if (nome == "Slime de Gelo")
    {
        return {
            "       _______",
            "     .'       '.",
            "    /  o     o  \\",
            "   |      ^      |",
            "    \\   '---'   /",
            "     '---------'" 
        };
    }

    if (nome == "Bandido")
    {
        return {
            "       ______",
            "      / ____ \\",
            "     | | -- | |",
            "     | |o  o| |",
            "     |   --   |",
            "      \\______/"
        };
    }

    return {
        "       _______",
        "     _/ _____ \\_",
        "    /  /     \\  \\",
        "   |  |  o o  |  |",
        "   |  |   ^   |  |",
        "    \\  \\_____/  /",
        "     '---------'" 
    };
}

void mostrarArte(std::ostream& saida, const std::vector<std::string>& arte,
                 const std::string& cor)
{
    saida << cor;
    for (const auto& linhaArte : arte)
    {
        saida << linhaArte << '\n';
    }
    saida << reset;
}

} // namespace termu::visual
