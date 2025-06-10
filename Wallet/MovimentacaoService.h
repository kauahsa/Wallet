
#pragma once
#include "IMovimentacaoDAO.h"
#include "ICarteiraDAO.h"
#include <memory>

namespace ft_coin {
    class MovimentacaoService {
    public:
        MovimentacaoService(std::unique_ptr<IMovimentacaoDAO> movDAO, ICarteiraDAO* cartDAO);
        void registrarCompra(int idCarteira, double quantidade, const std::string& data);
        void registrarVenda(int idCarteira, double quantidade, const std::string& data);
    private:
        std::unique_ptr<IMovimentacaoDAO> movimentacaoDAO;
        ICarteiraDAO* carteiraDAO; // Raw pointer, não possui ownership
    };
}