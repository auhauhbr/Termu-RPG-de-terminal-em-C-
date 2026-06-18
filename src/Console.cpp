#include "termu/Console.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

namespace termu {

void configurarConsole() {
#ifdef _WIN32
    // o Windows pode abrir com outra codificação, aí "físico" vira uma bagunça
    // então forço UTF-8 tanto pra saída como pra entrada
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE saida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;

    if (GetConsoleMode(saida, &modo)) {
        SetConsoleMode(saida, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

} // namespace termu
