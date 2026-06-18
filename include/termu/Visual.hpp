#pragma once

#include "termu/Tipos.hpp"

#include <ostream>
#include <string>
#include <vector>

namespace termu::visual {

inline constexpr const char* reset = "\033[0m";
inline constexpr const char* negrito = "\033[1m";
inline constexpr const char* vermelho = "\033[91m";
inline constexpr const char* verde = "\033[92m";
inline constexpr const char* amarelo = "\033[93m";
inline constexpr const char* azul = "\033[94m";
inline constexpr const char* magenta = "\033[95m";
inline constexpr const char* ciano = "\033[96m";
inline constexpr const char* cinza = "\033[90m";

void limpar(std::ostream& saida);
void linha(std::ostream& saida, int tamanho = 70);
void titulo(std::ostream& saida, const std::string& texto);
void opcao(std::ostream& saida, int numero, const std::string& texto,
           bool disponivel = true);

std::string barra(int atual, int maximo, int tamanho,
                  const std::string& cor);
std::string corVida(int atual, int maximo);
std::string corElemento(Elemento elemento);

std::vector<std::string> arteClasse(ClassePersonagem classePersonagem);
std::vector<std::string> arteMonstro(const std::string& nome);
void mostrarArte(std::ostream& saida, const std::vector<std::string>& arte,
                 const std::string& cor);

} // namespace termu::visual
