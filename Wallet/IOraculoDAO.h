#pragma once
#include "OraculoDTO.h"
#include <memory>
#include <string>

namespace ft_coin {
    class IOraculoDAO {
    public:
        virtual ~IOraculoDAO() = default;
        virtual std::unique_ptr<OraculoDTO> getCotacao(const std::string& data) = 0;
        virtual std::unique_ptr<OraculoDTO> getUltimaCotacao() = 0;
    };
}