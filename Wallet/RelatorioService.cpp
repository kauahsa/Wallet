#include "RelatorioService.h"
#include <algorithm>
#include <stdexcept>

namespace ft_coin {
    RelatorioService::RelatorioService(ICarteiraDAO* cartDAO, IMovimentacaoDAO* movDAO, IOraculoDAO* oraculoDAO)
        : carteiraDAO(cartDAO), movimentacaoDAO(movDAO), oraculoDAO(oraculoDAO) {
    }

    std::vector<CarteiraDTO> RelatorioService::listarCarteirasPorId() {
        auto carteiras = carteiraDAO->listarTodos();
        std::sort(carteiras.begin(), carteiras.end(), [](const CarteiraDTO& a, const CarteiraDTO& b) {
            return a.id < b.id;
            });
        return carteiras;
    }

    std::vector<CarteiraDTO> RelatorioService::listarCarteirasPorNome() {
        auto carteiras = carteiraDAO->listarTodos();
        std::sort(carteiras.begin(), carteiras.end(), [](const CarteiraDTO& a, const CarteiraDTO& b) {
            return a.nomeTitular < b.nomeTitular;
            });
        return carteiras;
    }

    double RelatorioService::getSaldoAtual(int idCarteira) {
        if (!carteiraDAO->recuperar(idCarteira)) {
            throw std::runtime_error("Carteira nao encontrada.");
        }
        double saldo = 0.0;
        auto historico = movimentacaoDAO->historicoPorCarteira(idCarteira);
        for (const auto& mov : historico) {
            if (mov.tipoOperacao == 'C') saldo += mov.quantidade;
            else if (mov.tipoOperacao == 'V') saldo -= mov.quantidade;
        }
        return saldo;
    }

    std::vector<MovimentacaoDTO> RelatorioService::getHistoricoMovimentacao(int idCarteira) {
        if (!carteiraDAO->recuperar(idCarteira)) {
            throw std::runtime_error("Carteira nao encontrada.");
        }
        return movimentacaoDAO->historicoPorCarteira(idCarteira);
    }

    GanhoPerdaInfo RelatorioService::calcularGanhoPerda(int idCarteira) {
        if (!carteiraDAO->recuperar(idCarteira)) {
            throw std::runtime_error("Carteira nao encontrada.");
        }

        auto historico = movimentacaoDAO->historicoPorCarteira(idCarteira);
        auto ultimaCotacaoDto = oraculoDAO->getUltimaCotacao();
        if (!ultimaCotacaoDto) {
            throw std::runtime_error("Nao foi possivel obter a cotacao atual do oraculo.");
        }
        double cotacaoAtual = ultimaCotacaoDto->cotacao;

        double custoTotal = 0.0;
        double receitaTotal = 0.0;
        double saldoMoedas = 0.0;

        for (const auto& mov : historico) {
            auto cotacaoDiaDto = oraculoDAO->getCotacao(mov.dataOperacao);
            if (!cotacaoDiaDto) {
                throw std::runtime_error("Nao ha cotacao para a data " + mov.dataOperacao + ". Impossivel calcular.");
            }
            double cotacaoDoDia = cotacaoDiaDto->cotacao;

            if (mov.tipoOperacao == 'C') {
                custoTotal += mov.quantidade * cotacaoDoDia;
                saldoMoedas += mov.quantidade;
            }
            else if (mov.tipoOperacao == 'V') {
                receitaTotal += mov.quantidade * cotacaoDoDia;
                saldoMoedas -= mov.quantidade;
            }
        }

        double valorAtualCarteira = saldoMoedas * cotacaoAtual;
        double ganhoPerdaRealizado = receitaTotal - custoTotal;

        GanhoPerdaInfo info;
        info.valorAtualCarteira = valorAtualCarteira;
        info.ganhoPerdaTotal = ganhoPerdaRealizado;

        return info;
    }
}