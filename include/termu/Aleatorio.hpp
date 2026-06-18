#pragma once

#include <cstdint>
#include <random>

namespace termu {

class Aleatorio {
public:
    explicit Aleatorio(std::uint32_t semente = std::random_device{}());

    int inteiro(int minimo, int maximo);
    double real(double minimo = 0.0, double maximo = 1.0);
    bool chance(double probability);

private:
    std::mt19937 motor_;
};

} // namespace termu
