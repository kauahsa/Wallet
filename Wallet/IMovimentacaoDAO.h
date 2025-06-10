#pragma once
#include "MovimentacaoDTO.h"
#include <vector>
#include <memory>

namespace ft_coin {
    class IMovimentacaoDAO {
    public:
        virtual ~IMovimentacaoDAO() = default;
        virtual void registrar(MovimentacaoDTO& movimentacao) = 0;
        virtual std::vector<MovimentacaoDTO> historicoPorCarteira(int idCarteira) = 0;
    };
}