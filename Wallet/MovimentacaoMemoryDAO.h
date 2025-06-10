#pragma once
#include "IMovimentacaoDAO.h"

namespace ft_coin {
    class MovimentacaoMemoryDAO : public IMovimentacaoDAO {
    public:
        void registrar(MovimentacaoDTO& movimentacao) override;
        std::vector<MovimentacaoDTO> historicoPorCarteira(int idCarteira) override;
    private:
        std::vector<MovimentacaoDTO> movimentacoes;
        int proximoId = 1;
    };
}