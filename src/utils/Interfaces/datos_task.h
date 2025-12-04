#ifndef DATOS_TASK_H
#define DATOS_TASK_H
#include <string>
#include <vector>
#include <variant>
#include <cryptopp/secblock.h>
#include <optional>

using reu_var = std::variant <

    bool,
    std::string,
    CryptoPP::SecByteBlock,
    std::vector<std::string>,
    std::monostate

>;

struct datos_task{

    reu_var resul_dt;
    std::optional<CryptoPP::SecByteBlock> clave;
    std::string path;
    std::optional<int> pro;

}; 

/*
* Habilita que qt lo reconozca como meta-type
*/
Q_DECLARE_METATYPE(datos_task)

#endif 

