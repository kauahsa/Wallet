#include "MariaDBDAO.h"
#include "CarteiraMemoryDAO.h"
#include "MovimentacaoMemoryDAO.h"
#include "OraculoMemoryDAO.h"
#include "CarteiraService.h"
#include "MovimentacaoService.h"
#include "RelatorioService.h"
#include "CLI.h"
#include <iostream>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace ft_coin;

// ##############################################################
// ### ALTERE AQUI PARA MUDAR A PERSISTENCIA                 ####
// ### 'true' para usar o banco MariaDB (remoto)             ####
// ### 'false' para usar a simulação em memória              ####
// ##############################################################
const bool USE_DATABASE = false;

int main() {
    #ifdef _WIN32
    
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
    #endif

    try {
        // --- Injeção de Dependência ---
        // Criação dos DAOs com base na flag USE_DATABASE
        std::unique_ptr<ICarteiraDAO> carteiraDAO;
        std::unique_ptr<IMovimentacaoDAO> movimentacaoDAO;
        std::unique_ptr<IOraculoDAO> oraculoDAO;

        if (USE_DATABASE) {
            std::cout << "Usando persistencia em Banco de Dados (MariaDB)..." << std::endl;
            carteiraDAO = std::make_unique<CarteiraMariaDBDAO>();
            movimentacaoDAO = std::make_unique<MovimentacaoMariaDBDAO>();
            oraculoDAO = std::make_unique<OraculoMariaDBDAO>();
        } else {
            std::cout << "Usando persistencia em Memoria..." << std::endl;
            carteiraDAO = std::make_unique<CarteiraMemoryDAO>();
            movimentacaoDAO = std::make_unique<MovimentacaoMemoryDAO>();
            oraculoDAO = std::make_unique<OraculoMemoryDAO>();
        }
        
        // Os serviços recebem ponteiros brutos para DAOs que eles não possuem,
        // exceto o primeiro serviço que assume a posse do ponteiro único.
        // Isso permite que múltiplos serviços compartilhem o mesmo DAO.
        ICarteiraDAO* pCarteiraDAO = carteiraDAO.get();
        IMovimentacaoDAO* pMovimentacaoDAO = movimentacaoDAO.get();
        IOraculoDAO* pOraculoDAO = oraculoDAO.get();
        
        // Criação dos Serviços
        auto carteiraService = std::make_unique<CarteiraService>(std::move(carteiraDAO));
        auto movimentacaoService = std::make_unique<MovimentacaoService>(std::move(movimentacaoDAO), pCarteiraDAO);
        auto relatorioService = std::make_unique<RelatorioService>(pCarteiraDAO, pMovimentacaoDAO, pOraculoDAO);
        
        // Criação e execução da Interface de Linha de Comando (CLI)
        CLI cli(std::move(carteiraService), std::move(movimentacaoService), std::move(relatorioService));
        cli.run();

    } catch (const std::exception& e) {
        std::cerr << "\nERRO FATAL NO SISTEMA: " << e.what() << std::endl;
        std::cerr << "O programa sera encerrado." << std::endl;
        return 1; // Retorna um código de erro
    }

    return 0; // Sucesso
}