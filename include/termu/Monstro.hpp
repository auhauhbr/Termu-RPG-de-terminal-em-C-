#pragma once

#include "termu/Catalogo.hpp"
#include "termu/Combatente.hpp"

#include <string>
#include <vector>

namespace termu {

class Monstro : public Combatente {
public:
    Monstro(const ModeloMonstro& definicao, int nivel);

    const std::string& nome() const override;
    const std::string& tipo() const;
    int nivel() const;
    int hpAtual() const override;
    const Atributos& atributos() const override;
    bool ehChefe() const;
    bool estaVivo() const override;
    PadraoIA padraoIA() const;
    int xpBase() const;
    int ouroMinimo() const;
    int ouroMaximo() const;
    double afinidade(Elemento elemento) const;
    const std::vector<StatusAtivo>& statusAtivos() const;

    void receberDano(int quantidade) override;
    void adicionarStatus(TipoStatus tipo, int duracao) override;
    bool impedeAcao() const;
    void processarFimTurno(std::vector<std::string>& registro) override;

private:
    std::string nome_;
    std::string tipo_;
    int nivel_;
    int hpAtual_;
    Atributos atributos_;
    Elemento fraqueza_;
    Elemento resistencia_;
    Elemento imunidade_;
    PadraoIA padraoIA_;
    int xpBase_;
    int ouroMinimo_;
    int ouroMaximo_;
    bool chefe_;
    std::vector<StatusAtivo> status_;
};

} // namespace termu
