#pragma once
#include "ICarteiraDAO.h"
#include <memory>

namespace ft_coin {
    class CarteiraService {
    public:
        CarteiraService(std::unique_ptr<ICarteiraDAO> dao);
        void criarCarteira(const std::string& nome, const std::string& corretora);
        std::unique_ptr<CarteiraDTO> buscarCarteira(int id);
        void atualizarCarteira(int id, const std::string& novoNome, const std::string& novaCorretora);
        void removerCarteira(int id);
        std::vector<CarteiraDTO> listarTodas();
    private:
        std::unique_ptr<ICarteiraDAO> carteiraDAO;
    };
}
