#ifndef GHJK_H
#define GHJK_H
#include <typeinfo>
#include <task.h>
#include <logging.h>

inline void loguearDatosTask(const datos_task& tarea, const char* contexto = "") {

    // Loguear path
    ut::Logger::Instancia().registrar(__FILE__, __LINE__, __FUNCTION__,
                                      std::string("[") + contexto + "] Path: " + tarea.path,INFO);

    // Loguear pro si existe
    if (tarea.pro.has_value()) {
        ut::Logger::Instancia().registrar(__FILE__, __LINE__, __FUNCTION__,
                                          std::string("[") + contexto + "] pro: " + std::to_string(tarea.pro.value()), INFO);
    } else {
        ut::Logger::Instancia().registrar(__FILE__, __LINE__, __FUNCTION__,
                                          std::string("[") + contexto + "] pro: nullopt", INFO);
    }

    // Loguear clave si existe
    if (tarea.clave.has_value()) {
        ut::Logger::Instancia().registrar(__FILE__, __LINE__, __FUNCTION__,
                                          std::string("[") + contexto + "] clave: SecByteBlock(" +
                                              std::to_string(tarea.clave->size()) + " bytes)", INFO);
    } else {
        ut::Logger::Instancia().registrar(__FILE__, __LINE__, __FUNCTION__,
                                          std::string("[") + contexto + "] clave: nullopt", INFO);
    }

    // Loguear resul_dt según su tipo actual
    if (std::holds_alternative<std::monostate>(tarea.resul_dt)) {
        ut::Logger::Instancia().registrar(__FILE__, __LINE__, __FUNCTION__,
                                          std::string("[") + contexto + "] resul_dt: monostate (vacío)", INFO);
    }
    else if (std::holds_alternative<bool>(tarea.resul_dt)) {
        bool val = std::get<bool>(tarea.resul_dt);
        ut::Logger::Instancia().registrar(__FILE__, __LINE__, __FUNCTION__,
                                          std::string("[") + contexto + "] resul_dt: bool = " + (val ? "true" : "false"), INFO);
    }
    else if (std::holds_alternative<std::string>(tarea.resul_dt)) {
        std::string val = std::get<std::string>(tarea.resul_dt);
        ut::Logger::Instancia().registrar(__FILE__, __LINE__, __FUNCTION__,
                                          std::string("[") + contexto + "] resul_dt: string = '" + val + "'", INFO);
    }
    else if (std::holds_alternative<CryptoPP::SecByteBlock>(tarea.resul_dt)) {
        auto& block = std::get<CryptoPP::SecByteBlock>(tarea.resul_dt);
        ut::Logger::Instancia().registrar(__FILE__, __LINE__, __FUNCTION__,
                                          std::string("[") + contexto + "] resul_dt: SecByteBlock(" +
                                              std::to_string(block.size()) + " bytes)", INFO);
    }
    else if (std::holds_alternative<std::vector<std::string>>(tarea.resul_dt)) {
        auto& vec = std::get<std::vector<std::string>>(tarea.resul_dt);
        ut::Logger::Instancia().registrar(__FILE__, __LINE__, __FUNCTION__,
                                          std::string("[") + contexto + "] resul_dt: vector<string> size=" +
                                              std::to_string(vec.size()), INFO);
    }
    else {
        ut::Logger::Instancia().registrar(__FILE__, __LINE__, __FUNCTION__,
                                          std::string("[") + contexto + "] resul_dt: tipo desconocido", INFO);
    }
}
#endif
