#pragma once
#include "CarteiraService.h"
#include "MovimentacaoService.h"
#include "RelatorioService.h"
#include <memory>

namespace ft_coin {
    class CLI {
    public:
        CLI(
            std::unique_ptr<CarteiraService> cs,
            std::unique_ptr<MovimentacaoService> ms,
            std::unique_ptr<RelatorioService> rs
        );
        void run();
    private:
        void exibirMenuPrincipal();
        void processarOpcaoPrincipal(char opcao);

        void menuCarteira();
        void processarOpcaoCarteira(char opcao);
        void incluirCarteira();
        void editarCarteira();
        void excluirCarteira();
        void buscarCarteira();

        void menuMovimentacao();
        void processarOpcaoMovimentacao(char opcao);
        void registrarCompra();
        void registrarVenda();

        void menuRelatorios();
        void processarOpcaoRelatorios(char opcao);
        void listarCarteirasOrdenadas();
        void exibirSaldoCarteira();
        void exibirHistoricoCarteira();
        void exibirGanhoPerda();

        void exibirAjuda();
        void exibirCreditos();

        // Funções utilitárias
        void limparTela();
        void pausar();
        void exibirHeader(const std::string& titulo);
        std::string getDataAtual();

        std::unique_ptr<CarteiraService> carteiraService;
        std::unique_ptr<MovimentacaoService> movimentacaoService;
        std::unique_ptr<RelatorioService> relatorioService;
        bool executando = true;
    };
}