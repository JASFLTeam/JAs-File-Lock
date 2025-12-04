#ifndef GESTOR_ARCH_H
#define GESTOR_ARCH_H
#include <fstream>
#include <filesystem>
#include <ctime>
#include <cryptopp/osrng.h>
#include "meta_dat.h"

namespace fs = std::filesystem;

class gestor_arch
{
public:

    gestor_arch() = default;

    meta_dat get_archivo(const std::string ruta);

    bool eliminar_seguro(const std::string ruta);

};

#endif
