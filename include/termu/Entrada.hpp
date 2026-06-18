#pragma once

#include <istream>
#include <optional>
#include <ostream>
#include <string>

namespace termu {

class Entrada {
public:
    Entrada(std::istream& entrada, std::ostream& saida);

    std::optional<int> lerInteiro(const std::string& pergunta, int minimo, int maximo);
    std::optional<std::string> lerNome(const std::string& pergunta, std::size_t tamanhoMaximo);
    std::optional<bool> confirmar(const std::string& pergunta);

private:
    std::istream& entrada_;
    std::ostream& saida_;
};

} // namespace termu
