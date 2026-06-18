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
#endif
}

} // namespace termu
