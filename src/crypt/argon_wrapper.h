#ifndef ARGON_W_H
#define ARGON_W_H
#include <argon2/argon2.h>
#include <cryptopp/secblock.h>
#include <cryptopp/sha.h>
#include <cryptopp/hmac.h>
#include <cryptopp/osrng.h>
#include <cryptopp/drbg.h>
#include <cryptopp/argnames.h>

namespace ut {
    class argon2 {
        private:
            /*
            * Constantes preestablecidos
            * memoria = 64mb
            * tipo = argon2id
            * iteracion = 3
            */
            const size_t hash_lg = 32;
            const size_t salt_lg = 16;
            const uint32_t iteracion = 3;
            const uint32_t mem_costo = 65536;
            const uint32_t hilo = 2;

        public:
            argon2() = default;

            /*
            * genera un hash codificado tomando una clave, devolviendo un tipo SecByteBlock con el hash
            */
            int hash_2id(const CryptoPP::SecByteBlock& clave, CryptoPP::SecByteBlock& salida, CryptoPP::SecByteBlock& _salt);
    };
}

#endif
