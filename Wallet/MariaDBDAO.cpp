#include "MariaDBDAO.h"
#include "DatabaseConfig.h"
#include <stdexcept>

namespace ft_coin {
    // Implementação da função estática para criar conexão
    std::unique_ptr<sql::Connection> MariaDBDAO::createConnection() {
        try {
            sql::Driver* driver = sql::mariadb::get_driver_instance();
            sql::SQLString url("jdbc:mariadb://" + DB_HOST + ":3306/" + DB_NAME);
            sql::Properties properties({ {"user", DB_USER}, {"password", DB_PASS} });
            return std::unique_ptr<sql::Connection>(driver->connect(url, properties));
        }
        catch (sql::SQLException& e) {
            throw std::runtime_error("Falha na conexao com o banco: " + std::string(e.what()));
        }
    }

    // --- Implementação CarteiraMariaDBDAO ---
    void CarteiraMariaDBDAO::incluir(CarteiraDTO& carteira) {
        try {
            auto conn = createConnection();
            std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO CARTEIRA(Titular, Corretora) VALUES (?, ?)"));
            pstmt->setString(1, carteira.nomeTitular);
            pstmt->setString(2, carteira.corretora);
            pstmt->executeUpdate();

            // Recupera o ID gerado
            std::unique_ptr<sql::Statement> stmt(conn->createStatement());
            std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT LAST_INSERT_ID()"));
            if (res->next()) {
                carteira.id = res->getInt(1);
            }
        }
        catch (sql::SQLException& e) {
            throw std::runtime_error("Erro ao inserir carteira: " + std::string(e.what()));
        }
    }

    std::unique_ptr<CarteiraDTO> CarteiraMariaDBDAO::recuperar(int id) {
        try {
            auto conn = createConnection();
            std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT IdCarteira, Titular, Corretora FROM CARTEIRA WHERE  IdCarteira = ?"));
            pstmt->setInt(1, id);
            std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

            if (res->next()) {
                auto dto = std::make_unique<CarteiraDTO>();
                dto->id = res->getInt("IdCarteira");
                dto->nomeTitular = res->getString("Titular");
                dto->corretora = res->getString("Corretora");
                return dto;
            }
        }
        catch (sql::SQLException& e) {
            throw std::runtime_error("Erro ao recuperar carteira: " + std::string(e.what()));
        }
        return nullptr;
    }

    void CarteiraMariaDBDAO::editar(const CarteiraDTO& carteira) {
        try {
            auto conn = createConnection();
            std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("UPDATE CARTEIRA SET Titular = ?, Corretora = ? WHERE  IdCarteira = ?"));
            pstmt->setString(1, carteira.nomeTitular);
            pstmt->setString(2, carteira.corretora);
            pstmt->setInt(3, carteira.id);
            if (pstmt->executeUpdate() == 0) {
                throw std::runtime_error("Carteira nao encontrada para edicao.");
            }
        }
        catch (sql::SQLException& e) {
            throw std::runtime_error("Erro ao editar carteira: " + std::string(e.what()));
        }
    }

    void CarteiraMariaDBDAO::excluir(int id) {
        try {
            auto conn = createConnection();
            std::unique_ptr<sql::PreparedStatement> delMovPstmt(conn->prepareStatement("DELETE FROM MOVIMENTACAO WHERE  IdCarteira = ?"));
            delMovPstmt->setInt(1, id);
            delMovPstmt->executeUpdate();

            std::unique_ptr<sql::PreparedStatement> delCartPstmt(conn->prepareStatement("DELETE FROM CARTEIRA WHERE  IdCarteira = ?"));
            delCartPstmt->setInt(1, id);
            if (delCartPstmt->executeUpdate() == 0) {
                throw std::runtime_error("Carteira nao encontrada para exclusao.");
            }
        }
        catch (sql::SQLException& e) {
            throw std::runtime_error("Erro ao excluir carteira: " + std::string(e.what()));
        }
    }

    std::vector<CarteiraDTO> CarteiraMariaDBDAO::listarTodos() {
        std::vector<CarteiraDTO> lista;
        try {
            auto conn = createConnection();
            std::unique_ptr<sql::Statement> stmt(conn->createStatement());
            std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT  IdCarteira, Titular, Corretora FROM CARTEIRA"));
            while (res->next()) {
                CarteiraDTO dto;
                dto.id = res->getInt("IdCarteira");
                dto.nomeTitular = res->getString("Titular");
                dto.corretora = res->getString("Corretora");
                lista.push_back(dto);
            }
        }
        catch (sql::SQLException& e) {
            throw std::runtime_error("Erro ao listar carteiras: " + std::string(e.what()));
        }
        return lista;
    }

    // --- Implementação MovimentacaoMariaDBDAO ---
    void MovimentacaoMariaDBDAO::registrar(MovimentacaoDTO& movimentacao) {
        try {
            auto conn = createConnection();
            std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
                "INSERT INTO MOVIMENTACAO(IdCarteira, Data, TipoOperacao, Quantidade) VALUES (?, ?, ?, ?)"
            ));
            pstmt->setInt(1, movimentacao.idCarteira);
            pstmt->setString(2, movimentacao.dataOperacao);
            pstmt->setString(3, std::string(1, movimentacao.tipoOperacao));
            pstmt->setDouble(4, movimentacao.quantidade);
            pstmt->executeUpdate();

            std::unique_ptr<sql::Statement> stmt(conn->createStatement());
            std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT LAST_INSERT_ID()"));
            if (res->next()) {
                movimentacao.id = res->getInt(1);
            }
        }
        catch (sql::SQLException& e) {
            throw std::runtime_error("Erro ao registrar movimentacao: " + std::string(e.what()));
        }
    }

    std::vector<MovimentacaoDTO> MovimentacaoMariaDBDAO::historicoPorCarteira(int idCarteira) {
        std::vector<MovimentacaoDTO> historico;
        try {
            auto conn = createConnection();
            std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
                "SELECT IdMovimento, Data, TipoOperacao, Quantidade FROM MOVIMENTACAO WHERE IdCarteira = ? ORDER BY Data"
            ));
            pstmt->setInt(1, idCarteira);
            std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
            while (res->next()) {
                MovimentacaoDTO dto;
                dto.id = res->getInt("IdMovimento");
                dto.idCarteira = idCarteira;
                dto.dataOperacao = res->getString("Data");
                dto.tipoOperacao = res->getString("TipoOperacao")[0];
                dto.quantidade = res->getDouble("Quantidade");
                historico.push_back(dto);
            }
        }
        catch (sql::SQLException& e) {
            throw std::runtime_error("Erro ao buscar historico: " + std::string(e.what()));
        }
        return historico;
    }

    // --- Implementação OraculoMariaDBDAO ---
    std::unique_ptr<OraculoDTO> OraculoMariaDBDAO::getCotacao(const std::string& data) {
        try {
            auto conn = createConnection();
            std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT Cotacao FROM ORACULO WHERE data = ?"));
            pstmt->setString(1, data);
            std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

            if (res->next()) {
                auto dto = std::make_unique<OraculoDTO>();
                dto->data = data;
                dto->cotacao = res->getDouble("Cotacao");
                return dto;
            }
        }
        catch (sql::SQLException& e) {
            throw std::runtime_error("Erro ao recuperar cotacao: " + std::string(e.what()));
        }
        return nullptr;
    }

    std::unique_ptr<OraculoDTO> OraculoMariaDBDAO::getUltimaCotacao() {
        try {
            auto conn = createConnection();
            std::unique_ptr<sql::Statement> stmt(conn->createStatement());
            std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT Data, Cotacao FROM ORACULO ORDER BY Data DESC LIMIT 1"));

            if (res->next()) {
                auto dto = std::make_unique<OraculoDTO>();
                dto->data = res->getString("Data");
                dto->cotacao = res->getDouble("Cotacao");
                return dto;
            }
        }
        catch (sql::SQLException& e) {
            throw std::runtime_error("Erro ao recuperar ultima cotacao: " + std::string(e.what()));
        }
        return nullptr;
    }
}
