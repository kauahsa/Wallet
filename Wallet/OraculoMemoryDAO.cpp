#include "OraculoMemoryDAO.h"
#include <chrono>
#include <iomanip>
#include <sstream>

namespace ft_coin {
    OraculoMemoryDAO::OraculoMemoryDAO() {
        // Populando com dados de exemplo
        cotacoes["2025-05-01"] = { "2025-05-01", 5.00 };
        cotacoes["2025-05-02"] = { "2025-05-02", 5.10 };
        cotacoes["2025-05-03"] = { "2025-05-03", 5.05 };
        cotacoes["2025-05-04"] = { "2025-05-04", 5.15 };
        cotacoes["2025-05-05"] = { "2025-05-05", 5.20 };
    }

    std::unique_ptr<OraculoDTO> OraculoMemoryDAO::getCotacao(const std::string& data) {
        auto it = cotacoes.find(data);
        if (it != cotacoes.end()) {
            return std::make_unique<OraculoDTO>(it->second);
        }
        return nullptr;
    }

    std::unique_ptr<OraculoDTO> OraculoMemoryDAO::getUltimaCotacao() {
        if (!cotacoes.empty()) {
            auto it = cotacoes.rbegin(); // O último elemento em um map ordenado
            return std::make_unique<OraculoDTO>(it->second);
        }
        // Retornar cotação padrão se o oráculo estiver vazio
        auto defaultCotacao = std::make_unique<OraculoDTO>();
        defaultCotacao->cotacao = 1.0;

        // Obtém data atual para o DTO
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        tm buf;
        localtime_s(&buf, &in_time_t);
        ss << std::put_time(&buf, "%Y-%m-%d");
        defaultCotacao->data = ss.str();

        return defaultCotacao;
    }
}