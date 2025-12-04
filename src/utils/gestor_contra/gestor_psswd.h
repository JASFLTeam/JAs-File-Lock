#ifndef GESTOR_PSSWD_H
#define GESTOR_PSSWD_H
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>

class Gestor_psswd{

    private:
        std::string hash_mast;
        bool sesion; // para saber si la sesion esta abierta

    public:
    
        Gestor_psswd();

    bool autenticar(const std::string& psswd_in);

    std::string encry_data(const std::string& data_in, const std::string& key);

    std::string decry_data(const std::string& data_cifrada, const std::string& key);
    
    std::string hasher_data(const std::string& psswd_maestra);


};

#endif
