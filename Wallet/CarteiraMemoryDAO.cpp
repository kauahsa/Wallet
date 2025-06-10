#include "CarteiraMemoryDAO.h"
#include <stdexcept>
#include <algorithm>

namespace ft_coin {
    void CarteiraMemoryDAO::incluir(CarteiraDTO& carteira) {
        carteira.id = proximoId++;
        carteiras[carteira.id] = carteira;
    }

    std::unique_ptr<CarteiraDTO> CarteiraMemoryDAO::recuperar(int id) {
        auto it = carteiras.find(id);
        if (it != carteiras.end()) {
            return std::make_unique<CarteiraDTO>(it->second);
        }
        return nullptr;
    }

    void CarteiraMemoryDAO::editar(const CarteiraDTO& carteira) {
        if (carteiras.count(carteira.id)) {
            carteiras[carteira.id] = carteira;
        }
        else {
            throw std::runtime_error("Carteira nao encontrada para edicao.");
        }
    }

    void CarteiraMemoryDAO::excluir(int id) {
        if (carteiras.erase(id) == 0) {
            throw std::runtime_error("Carteira nao encontrada para exclusao.");
        }
    }

    std::vector<CarteiraDTO> CarteiraMemoryDAO::listarTodos() {
        std::vector<CarteiraDTO> lista;
        for (const auto& par : carteiras) {
            lista.push_back(par.second);
        }
        return lista;
    }
}