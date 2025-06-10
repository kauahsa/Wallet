#include "MovimentacaoService.h"
#include <stdexcept>

namespace ft_coin {
    MovimentacaoService::MovimentacaoService(std::unique_ptr<IMovimentacaoDAO> movDAO, ICarteiraDAO* cartDAO)
        : movimentacaoDAO(std::move(movDAO)), carteiraDAO(cartDAO) {
    }

    void MovimentacaoService::registrarCompra(int idCarteira, double quantidade, const std::string& data) {
        if (quantidade <= 0) {
            throw std::invalid_argument("Quantidade deve ser positiva.");
        }
        if (!carteiraDAO->recuperar(idCarteira)) {
            throw std::runtime_error("Carteira de destino nao existe.");
        }
        MovimentacaoDTO mov;
        mov.idCarteira = idCarteira;
        mov.quantidade = quantidade;
        mov.dataOperacao = data;
        mov.tipoOperacao = 'C';
        movimentacaoDAO->registrar(mov);
    }

    void MovimentacaoService::registrarVenda(int idCarteira, double quantidade, const std::string& data) {
        if (quantidade <= 0) {
            throw std::invalid_argument("Quantidade deve ser positiva.");
        }
        if (!carteiraDAO->recuperar(idCarteira)) {
            throw std::runtime_error("Carteira de origem nao existe.");
        }
        // Lógica de validação de saldo
        double saldoAtual = 0.0;
        auto historico = movimentacaoDAO->historicoPorCarteira(idCarteira);
        for (const auto& mov : historico) {
            if (mov.tipoOperacao == 'C') saldoAtual += mov.quantidade;
            else if (mov.tipoOperacao == 'V') saldoAtual -= mov.quantidade;
        }
        if (saldoAtual < quantidade) {
            throw std::runtime_error("Saldo insuficiente para realizar a venda.");
        }

        MovimentacaoDTO mov;
        mov.idCarteira = idCarteira;
        mov.quantidade = quantidade;
        mov.dataOperacao = data;
        mov.tipoOperacao = 'V';
        movimentacaoDAO->registrar(mov);
    }
}
