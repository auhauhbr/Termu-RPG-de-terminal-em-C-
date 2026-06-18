#include "termu/Monstro.hpp"

#include <algorithm>

namespace termu {

Monstro::Monstro(const ModeloMonstro& definicao, int nivel)
    : nome_(definicao.nome), tipo_(definicao.tipo), nivel_(std::max(1, nivel)),
      fraqueza_(definicao.fraqueza), resistencia_(definicao.resistencia),
      imunidade_(definicao.imunidade), padraoIA_(definicao.ai), xpBase_(definicao.xpBase),
      ouroMinimo_(definicao.ouroMinimo), ouroMaximo_(definicao.ouroMaximo),
      chefe_(definicao.chefe) {
    const double ganhos = nivel_ - 1;
    atributos_ = definicao.atributosBase;
    atributos_.hpMaximo += definicao.crescimentoPorNivel.hpMaximo * ganhos;
    atributos_.ataque += definicao.crescimentoPorNivel.ataque * ganhos;
    atributos_.magia += definicao.crescimentoPorNivel.magia * ganhos;
    atributos_.defesa += definicao.crescimentoPorNivel.defesa * ganhos;
    atributos_.resistencia += definicao.crescimentoPorNivel.resistencia * ganhos;
    atributos_.velocidade += definicao.crescimentoPorNivel.velocidade * ganhos;
    if (chefe_) {
        atributos_.hpMaximo *= 1.25;
        atributos_.ataque *= 1.15;
        atributos_.magia *= 1.15;
    }
    hpAtual_ = static_cast<int>(atributos_.hpMaximo);
}

const std::string& Monstro::nome() const { return nome_; }
const std::string& Monstro::tipo() const { return tipo_; }
int Monstro::nivel() const { return nivel_; }
int Monstro::hpAtual() const { return hpAtual_; }
const Atributos& Monstro::atributos() const { return atributos_; }
bool Monstro::ehChefe() const { return chefe_; }
bool Monstro::estaVivo() const { return hpAtual_ > 0; }
PadraoIA Monstro::padraoIA() const { return padraoIA_; }
int Monstro::xpBase() const { return xpBase_; }
int Monstro::ouroMinimo() const { return ouroMinimo_; }
int Monstro::ouroMaximo() const { return ouroMaximo_; }
const std::vector<StatusAtivo>& Monstro::statusAtivos() const { return status_; }

double Monstro::afinidade(Elemento elemento) const {
    if (elemento == imunidade_) return 0.0;
    if (elemento == fraqueza_) return 1.5;
    if (elemento == resistencia_) return 0.5;
    return 1.0;
}

void Monstro::receberDano(int quantidade) {
    hpAtual_ = std::max(0, hpAtual_ - std::max(0, quantidade));
}

void Monstro::adicionarStatus(TipoStatus tipo, int duracao) {
    for (auto& status : status_) {
        if (status.tipo == tipo) {
            status.turnosRestantes = duracao;
            return;
        }
    }
    status_.push_back({tipo, duracao});
}

bool Monstro::impedeAcao() const {
    return std::any_of(status_.begin(), status_.end(),
        [](const StatusAtivo& status) { return status.tipo == TipoStatus::Atordoamento; });
}

void Monstro::processarFimTurno(std::vector<std::string>& registro) {
    for (auto& status : status_) {
        if (status.tipo == TipoStatus::Queimadura) {
            const int dano = std::max(1, static_cast<int>(atributos_.hpMaximo * 0.05));
            receberDano(dano);
            registro.push_back(nome_ + " sofreu " + std::to_string(dano) + " por Queimadura.");
        } else if (status.tipo == TipoStatus::Veneno) {
            const int dano = std::max(1, static_cast<int>(atributos_.hpMaximo * 0.04));
            receberDano(dano);
            registro.push_back(nome_ + " sofreu " + std::to_string(dano) + " por Veneno.");
        }
        --status.turnosRestantes;
    }
    status_.erase(std::remove_if(status_.begin(), status_.end(),
        [](const StatusAtivo& status) { return status.turnosRestantes <= 0; }), status_.end());
}

} // namespace termu
