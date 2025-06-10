#pragma once
#include "ICarteiraDAO.h"
#include <map>

namespace ft_coin {
    class CarteiraMemoryDAO : public ICarteiraDAO {
    public:
        void incluir(CarteiraDTO& carteira) override;
        std::unique_ptr<CarteiraDTO> recuperar(int id) override;
        void editar(const CarteiraDTO& carteira) override;
        void excluir(int id) override;
        std::vector<CarteiraDTO> listarTodos() override;
    private:
        std::map<int, CarteiraDTO> carteiras;
        int proximoId = 1;
    };
}