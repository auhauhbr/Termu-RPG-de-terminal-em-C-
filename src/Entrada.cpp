#include "termu/Entrada.hpp"

#include <algorithm>
#include <cctype>
#include <charconv>

namespace termu {

Entrada::Entrada(std::istream& entrada, std::ostream& saida) : entrada_(entrada), saida_(saida) {}

std::optional<int> Entrada::lerInteiro(const std::string& pergunta, int minimo, int maximo) {
    while (true) {
        saida_ << pergunta;
        std::string linha;
        if (!std::getline(entrada_, linha)) return std::nullopt;

        int valor = 0;
        const auto inicio = linha.data();
        const auto fim = linha.data() + linha.size();
        // usei from_chars pq ele não joga exceção igual stoi
        // e tbm barra umas entradas estranhas tipo "2abc"
        const auto resultado = std::from_chars(inicio, fim, valor);
        if (resultado.ec == std::errc{} && resultado.ptr == fim && valor >= minimo && valor <= maximo) {
            return valor;
        }
        saida_ << "[Erro] Opção inválida. Escolha um valor entre "
                << minimo << " e " << maximo << ".\n";
    }
}

std::optional<std::string> Entrada::lerNome(const std::string& pergunta, std::size_t tamanhoMaximo) {
    while (true) {
        saida_ << pergunta;
        std::string nome;
        if (!std::getline(entrada_, nome)) return std::nullopt;

        nome.erase(std::remove_if(nome.begin(), nome.end(), [](unsigned char personagem) {
            return std::iscntrl(personagem) != 0;
        }), nome.end());

        const auto primeiroCaractere = nome.find_first_not_of(" \t");
        const auto ultimoCaractere = nome.find_last_not_of(" \t");
        if (primeiroCaractere == std::string::npos) {
            saida_ << "[Erro] O nome não pode ser vazio.\n";
            continue;
        }
        nome = nome.substr(primeiroCaractere, ultimoCaractere - primeiroCaractere + 1);
        if (nome.size() > tamanhoMaximo) {
            saida_ << "[Erro] O nome deve ter no máximo " << tamanhoMaximo << " caracteres.\n";
            continue;
        }
        return nome;
    }
}

std::optional<bool> Entrada::confirmar(const std::string& pergunta) {
    while (true) {
        saida_ << pergunta << " [S/N]: ";
        std::string linha;
        if (!std::getline(entrada_, linha)) return std::nullopt;
        if (linha == "s" || linha == "S") return true;
        if (linha == "n" || linha == "N") return false;
        saida_ << "[Erro] Digite S para sim ou N para não.\n";
    }
}

} // namespace termu
