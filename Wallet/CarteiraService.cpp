#include "CarteiraService.h"
#include <stdexcept>

#include <vector>

namespace ft_coin {
    CarteiraService::CarteiraService(std::unique_ptr<ICarteiraDAO> dao) : carteiraDAO(std::move(dao)) {}

    void CarteiraService::criarCarteira(const std::string& nome, const std::string& corretora) {
        if (nome.empty() || corretora.empty()) {
            throw std::invalid_argument("Nome do titular e corretora nao podem ser vazios.");
        }
        CarteiraDTO novaCarteira;
        novaCarteira.nomeTitular = nome;
        novaCarteira.corretora = corretora;
        carteiraDAO->incluir(novaCarteira);
    }

    std::unique_ptr<CarteiraDTO> CarteiraService::buscarCarteira(int id) {
        return carteiraDAO->recuperar(id);
    }

    void CarteiraService::atualizarCarteira(int id, const std::string& novoNome, const std::string& novaCorretora) {
        if (novoNome.empty() || novaCorretora.empty()) {
            throw std::invalid_argument("Nome e corretora nao podem ser vazios.");
        }
        auto carteira = carteiraDAO->recuperar(id);
        if (!carteira) {
            throw std::runtime_error("Carteira nao encontrada.");
        }
        carteira->nomeTitular = novoNome;
        carteira->corretora = novaCorretora;
        carteiraDAO->editar(*carteira);
    }

    void CarteiraService::removerCarteira(int id) {
        carteiraDAO->excluir(id);
    }

    std::vector<CarteiraDTO> CarteiraService::listarTodas() {
        return carteiraDAO->listarTodos();
    }
}