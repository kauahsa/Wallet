#pragma once
#include "CarteiraDTO.h"
#include <vector>
#include <memory>

namespace ft_coin {
    class ICarteiraDAO {
    public:
        virtual ~ICarteiraDAO() = default;
        virtual void incluir(CarteiraDTO& carteira) = 0;
        virtual std::unique_ptr<CarteiraDTO> recuperar(int id) = 0;
        virtual void editar(const CarteiraDTO& carteira) = 0;
        virtual void excluir(int id) = 0;
        virtual std::vector<CarteiraDTO> listarTodos() = 0;
    };
}