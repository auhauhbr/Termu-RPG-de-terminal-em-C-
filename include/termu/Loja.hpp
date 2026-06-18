#pragma once

#include "termu/Personagem.hpp"

#include <map>
#include <string>

namespace termu {

class Loja {
public:
    Loja();

    int estoqueRestante(const std::string& idItem) const;
    bool comprar(Personagem& personagem, const std::string& idItem, int quantidade, std::string& mensagem);
    bool vender(Personagem& personagem, const std::string& idItem, int quantidade, std::string& mensagem);

private:
    std::map<std::string, int> estoque_;
};

} // namespace termu
