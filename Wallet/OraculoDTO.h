#pragma once
#include <string>

namespace ft_coin {
    struct OraculoDTO {
        std::string data; // Formato YYYY-MM-DD
        double cotacao = 0.0;
    };
}