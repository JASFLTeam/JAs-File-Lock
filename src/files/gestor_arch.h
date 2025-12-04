#ifndef GESTOR_ARCH_H
#define GESTOR_ARCH_H
#include <fstream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>
#include "meta_dat.h"

namespace fs = std::filesystem;

class gestor_arch
{
public:

    gestor_arch() = default;

    meta_dat get_archivo(const std::string ruta);

    bool eliminar_seguro(const std::string ruta);

    int escribir_final(const std::string ruta, const CryptoPP::SecByteBlock& bytes);

    int leer_final(const std::string ruta, CryptoPP::SecByteBlock& salida, const unsigned long tam);

};

#endif
