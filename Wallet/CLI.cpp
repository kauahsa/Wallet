#include "CLI.h"
#include <iostream>
#include <limits>
#include <string>
#include <iomanip>
#include <chrono>
#include <sstream>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

namespace ft_coin {
    // Cores (ANSI escape codes)
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";

    CLI::CLI(std::unique_ptr<CarteiraService> cs, std::unique_ptr<MovimentacaoService> ms, std::unique_ptr<RelatorioService> rs)
        : carteiraService(std::move(cs)), movimentacaoService(std::move(ms)), relatorioService(std::move(rs)) {
#ifdef _WIN32
        // Habilita processamento de sequências de escape ANSI no console do Windows
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
#endif
    }

    // Funções Utilitárias
    void CLI::limparTela() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void CLI::pausar() {
        std::cout << "\nPressione ENTER para continuar...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        getchar();
    }

    void CLI::exibirHeader(const std::string& titulo) {
        limparTela();
        std::cout << CYAN << "========================================\n";
        std::cout << "        FT_Coin - " << titulo << "\n";
        std::cout << "========================================\n\n" << RESET;
    }

    std::string CLI::getDataAtual() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        tm buf;
        localtime_s(&buf, &in_time_t);
        ss << std::put_time(&buf, "%Y-%m-%d");
        return ss.str();
    }

    // Lógica principal da CLI
    void CLI::run() {
        while (executando) {
            exibirMenuPrincipal();
            char opcao;
            std::cout << "Escolha uma opcao: ";
            std::cin >> opcao;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer de entrada
            processarOpcaoPrincipal(opcao);
        }
    }

    void CLI::exibirMenuPrincipal() {
        exibirHeader("Menu Principal");
        std::cout << YELLOW << "1. " << RESET << "Gerenciar Carteiras\n";
        std::cout << YELLOW << "2. " << RESET << "Registrar Movimentacao\n";
        std::cout << YELLOW << "3. " << RESET << "Emitir Relatorios\n";
        std::cout << YELLOW << "4. " << RESET << "Ajuda\n";
        std::cout << YELLOW << "5. " << RESET << "Creditos\n";
        std::cout << RED << "S. " << RESET << "Sair\n\n";
    }

    void CLI::processarOpcaoPrincipal(char opcao) {
        opcao = toupper(opcao);
        switch (opcao) {
        case '1': menuCarteira(); break;
        case '2': menuMovimentacao(); break;
        case '3': menuRelatorios(); break;
        case '4': exibirAjuda(); break;
        case '5': exibirCreditos(); break;
        case 'S': executando = false; std::cout << "\nSaindo do sistema. Ate logo!\n"; break;
        default: std::cout << RED << "Opcao invalida!" << RESET << "\n"; pausar(); break;
        }
    }

    // --- Seção Carteira ---
    void CLI::menuCarteira() {
        char opcao = ' ';
        while (opcao != 'V') {
            exibirHeader("Gerenciar Carteiras");
            std::cout << YELLOW << "1. " << RESET << "Incluir Nova Carteira\n";
            std::cout << YELLOW << "2. " << RESET << "Editar Carteira Existente\n";
            std::cout << YELLOW << "3. " << RESET << "Excluir Carteira\n";
            std::cout << YELLOW << "4. " << RESET << "Buscar Carteira por ID\n";
            std::cout << RED << "V. " << RESET << "Voltar ao Menu Principal\n\n";
            std::cout << "Escolha uma opcao: ";
            std::cin >> opcao;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            processarOpcaoCarteira(opcao);
        }
    }

    void CLI::processarOpcaoCarteira(char opcao) {
        opcao = toupper(opcao);
        switch (opcao) {
        case '1': incluirCarteira(); break;
        case '2': editarCarteira(); break;
        case '3': excluirCarteira(); break;
        case '4': buscarCarteira(); break;
        case 'V': break;
        default: std::cout << RED << "Opcao invalida!" << RESET << "\n"; break;
        }
        if (opcao != 'V') pausar();
    }

    void CLI::incluirCarteira() {
        exibirHeader("Incluir Nova Carteira");
        std::string nome, corretora;
        std::cout << "Nome do Titular: ";
        std::getline(std::cin, nome);
        std::cout << "Nome da Corretora: ";
        std::getline(std::cin, corretora);
        try {
            carteiraService->criarCarteira(nome, corretora);
            std::cout << GREEN << "\nCarteira criada com sucesso!" << RESET << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << RED << "\nErro: " << e.what() << RESET << '\n';
        }
    }

    void CLI::editarCarteira() {
        exibirHeader("Editar Carteira");
        int id;
        std::cout << "ID da Carteira a ser editada: ";
        std::cin >> id;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        try {
            auto carteira = carteiraService->buscarCarteira(id);
            if (!carteira) {
                std::cout << RED << "Carteira nao encontrada." << RESET << "\n";
                return;
            }
            std::cout << "Titular atual: " << carteira->nomeTitular << ". Novo nome (deixe em branco para manter): ";
            std::string novoNome;
            std::getline(std::cin, novoNome);

            std::cout << "Corretora atual: " << carteira->corretora << ". Nova corretora (deixe em branco para manter): ";
            std::string novaCorretora;
            std::getline(std::cin, novaCorretora);

            if (novoNome.empty()) novoNome = carteira->nomeTitular;
            if (novaCorretora.empty()) novaCorretora = carteira->corretora;

            carteiraService->atualizarCarteira(id, novoNome, novaCorretora);
            std::cout << GREEN << "\nCarteira atualizada com sucesso!" << RESET << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << RED << "\nErro: " << e.what() << RESET << '\n';
        }
    }

    void CLI::excluirCarteira() {
        exibirHeader("Excluir Carteira");
        int id;
        std::cout << "ID da Carteira a ser excluida: ";
        std::cin >> id;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << YELLOW << "ATENCAO: Isso excluira a carteira e todas as suas movimentacoes. Deseja continuar? (s/n): " << RESET;
        char confirm;
        std::cin >> confirm;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (tolower(confirm) == 's') {
            try {
                carteiraService->removerCarteira(id);
                std::cout << GREEN << "\nCarteira excluida com sucesso!" << RESET << "\n";
            }
            catch (const std::exception& e) {
                std::cerr << RED << "\nErro: " << e.what() << RESET << '\n';
            }
        }
        else {
            std::cout << "\nOperacao cancelada.\n";
        }
    }

    void CLI::buscarCarteira() {
        exibirHeader("Buscar Carteira");
        int id;
        std::cout << "ID da Carteira: ";
        std::cin >> id;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        try {
            auto carteira = carteiraService->buscarCarteira(id);
            if (carteira) {
                std::cout << GREEN << "\n--- Dados da Carteira ---\n";
                std::cout << "ID: " << carteira->id << "\n";
                std::cout << "Titular: " << carteira->nomeTitular << "\n";
                std::cout << "Corretora: " << carteira->corretora << "\n" << RESET;
            }
            else {
                std::cout << RED << "\nCarteira com ID " << id << " nao encontrada." << RESET << "\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << RED << "\nErro: " << e.what() << RESET << '\n';
        }
    }


    // --- Seção Movimentação ---
    void CLI::menuMovimentacao() {
        char opcao = ' ';
        while (opcao != 'V') {
            exibirHeader("Registrar Movimentacao");
            std::cout << YELLOW << "1. " << RESET << "Registrar Compra\n";
            std::cout << YELLOW << "2. " << RESET << "Registrar Venda\n";
            std::cout << RED << "V. " << RESET << "Voltar ao Menu Principal\n\n";
            std::cout << "Escolha uma opcao: ";
            std::cin >> opcao;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            processarOpcaoMovimentacao(opcao);
        }
    }

    void CLI::processarOpcaoMovimentacao(char opcao) {
        opcao = toupper(opcao);
        switch (opcao) {
        case '1': registrarCompra(); break;
        case '2': registrarVenda(); break;
        case 'V': break;
        default: std::cout << RED << "Opcao invalida!" << RESET << "\n"; break;
        }
        if (opcao != 'V') pausar();
    }

    void CLI::registrarCompra() {
        exibirHeader("Registrar Compra");
        int idCarteira;
        double quantidade;
        std::string data;

        std::cout << "ID da Carteira: ";
        std::cin >> idCarteira;
        std::cout << "Quantidade comprada: ";
        std::cin >> quantidade;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Data da operacao (YYYY-MM-DD) [ENTER para data atual]: ";
        std::getline(std::cin, data);

        if (data.empty()) {
            data = getDataAtual();
        }

        try {
            movimentacaoService->registrarCompra(idCarteira, quantidade, data);
            std::cout << GREEN << "\nCompra registrada com sucesso!" << RESET << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << RED << "\nErro: " << e.what() << RESET << '\n';
        }
    }

    void CLI::registrarVenda() {
        exibirHeader("Registrar Venda");
        int idCarteira;
        double quantidade;
        std::string data;

        std::cout << "ID da Carteira: ";
        std::cin >> idCarteira;
        std::cout << "Quantidade vendida: ";
        std::cin >> quantidade;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Data da operacao (YYYY-MM-DD) [ENTER para data atual]: ";
        std::getline(std::cin, data);

        if (data.empty()) {
            data = getDataAtual();
        }

        try {
            movimentacaoService->registrarVenda(idCarteira, quantidade, data);
            std::cout << GREEN << "\nVenda registrada com sucesso!" << RESET << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << RED << "\nErro: " << e.what() << RESET << '\n';
        }
    }

    // --- Seção Relatórios ---
    void CLI::menuRelatorios() {
        char opcao = ' ';
        while (opcao != 'V') {
            exibirHeader("Emitir Relatorios");
            std::cout << YELLOW << "1. " << RESET << "Listar Carteiras (ordenado por ID ou Nome)\n";
            std::cout << YELLOW << "2. " << RESET << "Exibir Saldo Atual de uma Carteira\n";
            std::cout << YELLOW << "3. " << RESET << "Exibir Historico de Movimentacao\n";
            std::cout << YELLOW << "4. " << RESET << "Apresentar Ganho/Perda por Carteira\n";
            std::cout << RED << "V. " << RESET << "Voltar ao Menu Principal\n\n";
            std::cout << "Escolha uma opcao: ";
            std::cin >> opcao;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            processarOpcaoRelatorios(opcao);
        }
    }

    void CLI::processarOpcaoRelatorios(char opcao) {
        opcao = toupper(opcao);
        switch (opcao) {
        case '1': listarCarteirasOrdenadas(); break;
        case '2': exibirSaldoCarteira(); break;
        case '3': exibirHistoricoCarteira(); break;
        case '4': exibirGanhoPerda(); break;
        case 'V': break;
        default: std::cout << RED << "Opcao invalida!" << RESET << "\n"; break;
        }
        if (opcao != 'V') pausar();
    }

    void CLI::listarCarteirasOrdenadas() {
        exibirHeader("Listar Carteiras");
        std::cout << "Ordenar por (I)D ou (N)ome? ";
        char ordem;
        std::cin >> ordem;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        try {
            std::vector<CarteiraDTO> carteiras;
            if (toupper(ordem) == 'I') {
                carteiras = relatorioService->listarCarteirasPorId();
            }
            else {
                carteiras = relatorioService->listarCarteirasPorNome();
            }

            if (carteiras.empty()) {
                std::cout << "\nNenhuma carteira cadastrada.\n";
            }
            else {
                std::cout << "\n" << std::left << std::setw(5) << "ID" << std::setw(25) << "Titular" << std::setw(20) << "Corretora" << "\n";
                std::cout << "--------------------------------------------------\n";
                for (const auto& c : carteiras) {
                    std::cout << std::left << std::setw(5) << c.id
                        << std::setw(25) << c.nomeTitular
                        << std::setw(20) << c.corretora << "\n";
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << RED << "\nErro: " << e.what() << RESET << '\n';
        }
    }

    void CLI::exibirSaldoCarteira() {
        exibirHeader("Saldo da Carteira");
        int id;
        std::cout << "ID da Carteira: ";
        std::cin >> id;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        try {
            double saldo = relatorioService->getSaldoAtual(id);
            std::cout << GREEN << "\nSaldo atual da carteira " << id << ": " << std::fixed << std::setprecision(8) << saldo << " moedas." << RESET << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << RED << "\nErro: " << e.what() << RESET << '\n';
        }
    }

    void CLI::exibirHistoricoCarteira() {
        exibirHeader("Historico de Movimentacao");
        int id;
        std::cout << "ID da Carteira: ";
        std::cin >> id;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        try {
            auto historico = relatorioService->getHistoricoMovimentacao(id);
            if (historico.empty()) {
                std::cout << "\nNenhuma movimentacao para esta carteira.\n";
            }
            else {
                std::cout << "\n" << std::left
                    << std::setw(10) << "ID Mov."
                    << std::setw(15) << "Data"
                    << std::setw(10) << "Tipo"
                    << std::setw(20) << "Quantidade" << "\n";
                std::cout << "-----------------------------------------------------\n";
                for (const auto& mov : historico) {
                    std::cout << std::left
                        << std::setw(10) << mov.id
                        << std::setw(15) << mov.dataOperacao
                        << std::setw(10) << (mov.tipoOperacao == 'C' ? "Compra" : "Venda")
                        << std::fixed << std::setprecision(8) << std::setw(20) << mov.quantidade << "\n";
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << RED << "\nErro: " << e.what() << RESET << '\n';
        }
    }

    void CLI::exibirGanhoPerda() {
        exibirHeader("Relatorio de Ganhos e Perdas");
        int id;
        std::cout << "ID da Carteira: ";
        std::cin >> id;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        try {
            auto info = relatorioService->calcularGanhoPerda(id);
            std::cout << "\n--- Relatorio Financeiro para a Carteira " << id << " ---\n";
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "Valor Atual da Carteira (em R$): " << GREEN << "R$ " << info.valorAtualCarteira << RESET << "\n";
            std::cout << "Ganho/Perda Realizado (em R$): ";
            if (info.ganhoPerdaTotal >= 0) {
                std::cout << GREEN << "R$ " << info.ganhoPerdaTotal << " (Lucro)" << RESET << "\n";
            }
            else {
                std::cout << RED << "R$ " << info.ganhoPerdaTotal << " (Prejuizo)" << RESET << "\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << RED << "\nErro: " << e.what() << RESET << '\n';
        }
    }

    // --- Seção Ajuda e Créditos ---
    void CLI::exibirAjuda() {
        exibirHeader("Ajuda do Sistema");
        std::cout << "O FT_Coin eh um sistema para gerenciamento de carteiras de moedas virtuais.\n\n"
            << "Funcionalidades:\n"
            << "1. Gerenciar Carteiras: Permite criar, editar, excluir e visualizar carteiras.\n"
            << "   Uma carteira representa seus ativos em uma determinada corretora.\n\n"
            << "2. Registrar Movimentacao: Registre suas compras e vendas de moedas.\n"
            << "   As movimentacoes sao essenciais para calcular seu saldo e ganhos.\n\n"
            << "3. Emitir Relatorios: Obtenha informacoes valiosas sobre seus investimentos.\n"
            << "   - Listar Carteiras: Veja todas as suas carteiras.\n"
            << "   - Saldo Atual: Confira quantas moedas voce possui em uma carteira.\n"
            << "   - Historico: Visualize todas as transacoes de uma carteira.\n"
            << "   - Ganho/Perda: Calcule o resultado financeiro de suas operacoes.\n\n"
            << "O sistema utiliza um 'Oraculo' para obter a cotacao da moeda em Reais (R$)\n"
            << "em diferentes datas, o que eh crucial para o calculo de ganhos e perdas.\n";
        pausar();
    }

    void CLI::exibirCreditos() {
        exibirHeader("Creditos");
        std::cout << "Sistema de Apuracao de Ganhos e Perdas em Carteira de Moedas\n\n"
            << "Projeto para a disciplina SI300 - Programacao Orientada a Objetos I\n"
            << "Faculdade de Tecnologia - UNICAMP\n\n"
            << "Desenvolvido por: [ Kauã Andrade \n Lucca Modeneis Dalla Costa Ziani \n Guilherme Vicente Ramalho \n Gabriel Colombo \n Wesley SantAna ]\n"
            << "Copyright (c) 2025\n"
            << "Versao: 1.0.0\n";
        pausar();
    }
}