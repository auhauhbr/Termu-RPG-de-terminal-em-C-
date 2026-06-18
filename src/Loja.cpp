#include "termu/Loja.hpp"

#include "termu/Catalogo.hpp"

namespace termu {

Loja::Loja() {
    for (const auto& item : itensCatalogo()) estoque_[item.identificador] = item.estoque;
}

int Loja::estoqueRestante(const std::string& idItem) const {
    const auto it = estoque_.find(idItem);
    return it == estoque_.end() ? 0 : it->second;
}

bool Loja::comprar(Personagem& personagem, const std::string& idItem, int quantidade, std::string& mensagem) {
    const DefinicaoItem* item = buscarItem(idItem);
    if (!item || quantidade <= 0) {
        mensagem = "Quantidade ou item inválido.";
        return false;
    }
    const int disponiveis = estoqueRestante(idItem);
    if (disponiveis != -1 && disponiveis < quantidade) {
        mensagem = "Item fora de estoque.";
        return false;
    }
    const std::int64_t total = static_cast<std::int64_t>(item->precoCompra) * quantidade;
    // primeiro valida tudo e só dps mexe no ouro/inventário
    // assim não fica estado pela metade se a compra der ruim
    if (!personagem.gastarOuro(total)) {
        mensagem = "Ouro insuficiente. Custo: " + std::to_string(total) +
                  ", disponível: " + std::to_string(personagem.ouro()) + ".";
        return false;
    }
    personagem.inventario().adicionar(idItem, quantidade);
    if (disponiveis != -1) estoque_[idItem] -= quantidade;
    mensagem = item->nome + " comprado com sucesso.";
    return true;
}

bool Loja::vender(Personagem& personagem, const std::string& idItem, int quantidade, std::string& mensagem) {
    const DefinicaoItem* item = buscarItem(idItem);
    if (!item || quantidade <= 0 || personagem.inventario().quantidadeDe(idItem) < quantidade) {
        mensagem = "Quantidade inválida para venda.";
        return false;
    }
    if (!item->vendavel) {
        mensagem = "Este item não pode ser vendido.";
        return false;
    }
    personagem.inventario().remover(idItem, quantidade);
    personagem.adicionarOuro(static_cast<std::int64_t>(item->precoVenda) * quantidade);
    mensagem = item->nome + " vendido com sucesso.";
    return true;
}

} // namespace termu
