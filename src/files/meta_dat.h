#ifndef META_DAT_H
#define META_DAT_H

#include <string>
#include <ctime> // Para el manejo de fechas/tiempo


struct meta_dat {
    bool existe;
    std::string ruta_arch;
    long long tam_bytes;
    std::string fech_crea;
};

#endif
