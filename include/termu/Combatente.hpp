#pragma once

#include "termu/Tipos.hpp"

#include <string>
#include <vector>

namespace termu {

class Combatente {
public:
    virtual ~Combatente() = default;

    virtual const std::string& nome() const = 0;
    virtual int hpAtual() const = 0;
    virtual const Atributos& atributos() const = 0;
    virtual bool estaVivo() const = 0;

    virtual void receberDano(int quantidade) = 0;
    virtual void adicionarStatus(TipoStatus tipo, int duracao) = 0;
    virtual void processarFimTurno(std::vector<std::string>& registro) = 0;
};

} // namespace termu
