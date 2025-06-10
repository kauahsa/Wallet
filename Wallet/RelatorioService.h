#pragma once
#include "ICarteiraDAO.h"
#include "IMovimentacaoDAO.h"
#include "IOraculoDAO.h"
#include <memory>
#include <vector>

namespace ft_coin {
    struct GanhoPerdaInfo {
        double ganhoPerdaTotal;
        double valorAtualCarteira;
    };

    class RelatorioService {
    public:
        RelatorioService(ICarteiraDAO* cartDAO, IMovimentacaoDAO* movDAO, IOraculoDAO* oraculoDAO);

        std::vector<CarteiraDTO> listarCarteirasPorId();
        std::vector<CarteiraDTO> listarCarteirasPorNome();
        double getSaldoAtual(int idCarteira);
        std::vector<MovimentacaoDTO> getHistoricoMovimentacao(int idCarteira);
        GanhoPerdaInfo calcularGanhoPerda(int idCarteira);

    private:
        ICarteiraDAO* carteiraDAO;
        IMovimentacaoDAO* movimentacaoDAO;
        IOraculoDAO* oraculoDAO;
    };
}
