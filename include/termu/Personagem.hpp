#pragma once

#include "termu/Combatente.hpp"
#include "termu/Inventario.hpp"
#include "termu/Tipos.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace termu {

class Personagem : public Combatente {
public:
    Personagem(std::string nome, ClassePersonagem classePersonagem);

    const std::string& nome() const override;
    ClassePersonagem classePersonagem() const;
    const std::string& nomeRecurso() const;
    int nivel() const;
    std::int64_t xp() const;
    std::int64_t ouro() const;
    int pontosHabilidade() const;
    int hpAtual() const override;
    int recursoAtual() const;
    const Atributos& atributos() const override;
    bool estaVivo() const override;

    Inventario& inventario();
    const Inventario& inventario() const;
    Equipamento& equipamento();
    const Equipamento& equipamento() const;
    const std::map<std::string, int>& niveisHabilidades() const;
    const std::vector<StatusAtivo>& statusAtivos() const;

    void receberDano(int quantidade) override;
    void curar(int quantidade);
    bool gastarRecurso(int quantidade);
    void restaurarRecurso(int quantidade);
    void restaurarTudo();
    void adicionarOuro(std::int64_t quantidade);
    bool gastarOuro(std::int64_t quantidade);
    std::vector<std::string> adicionarXp(std::int64_t quantidade);
    bool evoluirHabilidade(const std::string& idHabilidade, std::string& mensagem);
    bool equiparItem(const std::string& idItem, std::string& mensagem);
    bool desequiparEspaco(EspacoEquipamento espaco, std::string& mensagem);
    bool usarConsumivel(const std::string& idItem, bool emCombate, std::string& mensagem);
    void adicionarStatus(TipoStatus tipo, int duracao) override;
    void processarFimTurno(std::vector<std::string>& registro) override;
    void regenerarNoTurno();

private:
    void recalcularAtributos();
    void desbloquearHabilidadesDisponiveis();

    std::string nome_;
    ClassePersonagem classe_;
    int nivel_{1};
    std::int64_t xp_{0};
    std::int64_t ouro_{120};
    int pontosHabilidade_{0};
    int hpAtual_{0};
    int recursoAtual_{0};
    Atributos atributos_;
    Inventario inventario_;
    Equipamento equipamento_;
    std::map<std::string, int> ranksHabilidades_;
    std::vector<StatusAtivo> status_;
};

} // namespace termu
