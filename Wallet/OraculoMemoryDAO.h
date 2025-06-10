#pragma once
#include "IOraculoDAO.h"
#include <map>

namespace ft_coin {
    class OraculoMemoryDAO : public IOraculoDAO {
    public:
        OraculoMemoryDAO();
        std::unique_ptr<OraculoDTO> getCotacao(const std::string& data) override;
        std::unique_ptr<OraculoDTO> getUltimaCotacao() override;
    private:
        std::map<std::string, OraculoDTO> cotacoes;
    };
}