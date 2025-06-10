#include "MovimentacaoMemoryDAO.h"

namespace ft_coin {
    void MovimentacaoMemoryDAO::registrar(MovimentacaoDTO& movimentacao) {
        movimentacao.id = proximoId++;
        movimentacoes.push_back(movimentacao);
    }

    std::vector<MovimentacaoDTO> MovimentacaoMemoryDAO::historicoPorCarteira(int idCarteira) {
        std::vector<MovimentacaoDTO> historico;
        for (const auto& mov : movimentacoes) {
            if (mov.idCarteira == idCarteira) {
                historico.push_back(mov);
            }
        }
        return historico;
    }
}