#include "argon_wrapper.h"

/*
* genera un hash codificado tomando una clave
*/
int ut::argon2::hash_2id(const CryptoPP::SecByteBlock& clave,
        CryptoPP::SecByteBlock& salida, CryptoPP::SecByteBlock& _salt, const argon_op& op) {

    CryptoPP::SecByteBlock salt(salt_lg);

    if (op == RANDHASH) {
        /*
        * Fuente de entropía para generar el salt
        * Y generación de salt 16 bytes
        */
        CryptoPP::AutoSeededRandomPool entropia;
        /*
        * Contenedor del hash resultante
        */
        entropia.GenerateBlock(salt, salt_lg);

        /*
        * Retorna el salt usado
        */
        _salt = salt;
    }
    else {
        salt = _salt;
    }

    CryptoPP::SecByteBlock derivada(hash_lg);
    /*
    * Generación del hash propiamente dicho
    */
    int resultado = argon2id_hash_raw(iteracion, mem_costo,
        hilo, clave.data(),
        clave.size(), salt.data(),
        salt.size(), reinterpret_cast<char*>(derivada.data()), 
        derivada.size()
    );

    salida = derivada;

    return resultado;
}
