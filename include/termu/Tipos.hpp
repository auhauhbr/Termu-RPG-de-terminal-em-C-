#pragma once

#include <string>

namespace termu {

enum class ClassePersonagem { Guerreiro, Mago, Ladino, Clerigo };
enum class Elemento { Fisico, Fogo, Gelo, Raio, Sagrado, Sombrio, Nenhum };
enum class TipoDano { Fisico, Magico };
enum class CategoriaItem { Arma, Armadura, Acessorio, Consumivel, Material };
enum class EspacoEquipamento { Arma, Armadura, Acessorio };
enum class TipoStatus { Queimadura, Veneno, Lentidao, Atordoamento, Nenhum };
enum class PadraoIA { Agressivo, Defensivo, Misto, Tanque, FasesChefe };
enum class ResultadoCombate { Vitoria, Derrota, Fugiu };

struct Atributos {
    double hpMaximo{0};
    double recursoMaximo{0};
    double ataque{0};
    double magia{0};
    double defesa{0};
    double resistencia{0};
    double velocidade{0};
    double chanceCritico{0};
    double chanceEsquiva{0};

    Atributos& operator+=(const Atributos& outro);
};

struct StatusAtivo {
    TipoStatus tipo{TipoStatus::Nenhum};
    int turnosRestantes{0};
};

std::string paraTexto(ClassePersonagem valor);
std::string paraTexto(Elemento valor);
std::string paraTexto(CategoriaItem valor);
std::string paraTexto(EspacoEquipamento valor);
std::string paraTexto(TipoStatus valor);

} // namespace termu
