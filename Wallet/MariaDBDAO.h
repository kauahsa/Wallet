#pragma once
#include <mariadb/conncpp.hpp>
#include "ICarteiraDAO.h"
#include "IMovimentacaoDAO.h"
#include "IOraculoDAO.h"
#include <memory>

namespace ft_coin {
    // Classe base para DAOs de MariaDB para gerenciar a conexão
    class MariaDBDAO {
    protected:
        static std::unique_ptr<sql::Connection> createConnection();
    };

    class CarteiraMariaDBDAO : public ICarteiraDAO, protected MariaDBDAO {
    public:
        void incluir(CarteiraDTO& carteira) override;
        std::unique_ptr<CarteiraDTO> recuperar(int id) override;
        void editar(const CarteiraDTO& carteira) override;
        void excluir(int id) override;
        std::vector<CarteiraDTO> listarTodos() override;
    };

    class MovimentacaoMariaDBDAO : public IMovimentacaoDAO, protected MariaDBDAO {
    public:
        void registrar(MovimentacaoDTO& movimentacao) override;
        std::vector<MovimentacaoDTO> historicoPorCarteira(int idCarteira) override;
    };

    class OraculoMariaDBDAO : public IOraculoDAO, protected MariaDBDAO {
    public:
        std::unique_ptr<OraculoDTO> getCotacao(const std::string& data) override;
        std::unique_ptr<OraculoDTO> getUltimaCotacao() override;
    };
}