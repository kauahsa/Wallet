#pragma once
#include <string>

namespace ft_coin {
    struct MovimentacaoDTO {
        int id = 0;
        int idCarteira = 0;
        std::string dataOperacao; // Formato YYYY-MM-DD
        char tipoOperacao = ' '; // 'C' para Compra, 'V' para Venda
        double quantidade = 0.0;
    };
}