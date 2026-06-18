#include "termu/Aleatorio.hpp"

namespace termu {

Aleatorio::Aleatorio(std::uint32_t semente) : motor_(semente) {}

int Aleatorio::inteiro(int minimo, int maximo) {
    return std::uniform_int_distribution<int>(minimo, maximo)(motor_);
}

double Aleatorio::real(double minimo, double maximo) {
    return std::uniform_real_distribution<double>(minimo, maximo)(motor_);
}

bool Aleatorio::chance(double probability) {
    return real() < probability;
}

} // namespace termu
