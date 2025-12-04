#ifndef SECBLOCK_DUMPER_H
#define SECBLOCK_DUMPER_H

#include <cryptopp/secblock.h>
#include <logging.h>  // Asumiendo que este es el encabezado de tu logger

#include <iomanip>
#include <sstream>

/**
 * @brief Registra el contenido de un SecByteBlock usando el sistema de logging.
 * 
 * Solo para depuración. No usar en producción con datos sensibles.
 * 
 * @param block   El bloque seguro a registrar.
 * @param context Descripción opcional (ej: "Clave generada", "Salt").
 */
inline void dump_secbyteblock(const CryptoPP::SecByteBlock& block, const char* context = "") {
    std::ostringstream oss;
    if (context && context[0] != '\0') {
        oss << context << " | ";
    }
    oss << "Tamaño=" << block.size() << " bytes | Datos=";
    
    for (size_t i = 0; i < block.size(); ++i) {
        oss << std::hex << std::setfill('0') << std::setw(2)
            << static_cast<int>(block[i]);
        if (i < block.size() - 1) oss << " ";
    }

    // Usar tu sistema de logging
    ut::Logger::Instancia().registrar(__FILE__, __LINE__, __FUNCTION__, oss.str(), INFO);
}

#endif // SECBLOCK_DUMPER_H