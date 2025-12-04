#include "argon_wrapper.h"

/*
* genera un hash codificado tomando una clave
*/
int ut::argon2::hash_2id(CryptoPP::SecByteBlock& clave, CryptoPP::SecByteBlock& salida) {

    /*
    * Fuente de entropía para generar el salt
    */
    CryptoPP::AutoSeededRandomPool entropia;
    CryptoPP::SecByteBlock random(salt_lg);
    entropia.GenerateBlock(random, salt_lg);
    /*
    * generar salt de longitud 16 bytes
    * mediante el algoritmo HMAC_DRBG con SHA256
    */
    CryptoPP::SecByteBlock salt(salt_lg);
    CryptoPP::HMAC_DRBG<CryptoPP::SHA256> algoritmo(random, salt_lg);
    algoritmo.GenerateBlock(salt, salt_lg);

    /*
    * Contenedor del hash resultante
    */
    size_t longitud = argon2_encodedlen(iteracion, mem_costo,
        hilo, salt_lg,
        hash_lg, argon_tipo
    );
    CryptoPP::SecByteBlock derivada(longitud);
    /*
    * Generación del hash propiamente dicho
    */
    int resultado = argon2id_hash_encoded(iteracion, mem_costo,
        hilo, clave.data(),
        clave.size(), salt.data(),
        salt.size(), hash_lg,
        reinterpret_cast<char*>(derivada.data()), 
        derivada.size()
    );

    return resultado;
}
