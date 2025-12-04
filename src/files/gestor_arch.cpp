#include "gestor_arch.h"
#include "meta_dat.h"
#include <ctime>
#include <chrono>

meta_dat gestor_arch::get_archivo(const std::string ruta) {
    meta_dat salida;
    salida.existe = fs::exists(ruta);
    if (salida.existe) {
        /*
        * Los metadatos
        */
        salida.ruta_arch = ruta;
        salida.tam_bytes = fs::file_size(ruta);
        
        /*
        * Salida de fecha de modificación
        */
        auto ftime = fs::last_write_time(ruta);
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                        ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
                    );
        std::time_t c_time = std::chrono::system_clock::to_time_t(sctp);

        char buffer[100];
        if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&c_time))) {
            salida.fech_crea = buffer;
        } else {
            salida.fech_crea = std::string();
        }

    }
    return salida;
}

bool gestor_arch::eliminar_seguro(const std::string ruta) {
    try {
        /*
        * Obtener el tamaño del archivo
        */
        size_t file_size = fs::file_size(ruta);

        /*
        * Generar datos aleatorios seguros para la sobreescritura
        */
        CryptoPP::AutoSeededRandomPool rng;
        std::vector<char> wipe_buffer(file_size);
        rng.GenerateBlock(reinterpret_cast<CryptoPP::byte*>(wipe_buffer.data()), wipe_buffer.size());

        /*
        * Sobreescribir el archivo completo (Una pasada de datos aleatorios)
        */
        std::ofstream ofs(ruta, std::ios::binary | std::ios::trunc);
        if (!ofs.is_open()) {
            return false;
        }
        
        /*
        * Escribe los datos aleatorios sobre todo el contenido para eliminar rastros
        */
        ofs.write(wipe_buffer.data(), wipe_buffer.size());
        
        /*
        * Aseguramos que los datos se han escrito en el disco antes de cerrarlo
        */
        ofs.flush();
        ofs.close();

        /*
        * Eliminar el archivo del sistema (Una vez que el contenido es basura aleatoria)
        */
        return fs::remove(ruta);

    } catch (const fs::filesystem_error& e) {
        return false;
    }
}

/*
* Escribe Bytes al final del archivo
*/
int gestor_arch::escribir_final(const std::string ruta, const CryptoPP::SecByteBlock& bytes) {
    try {
        /*
        * Se carga para ser escrito
        */
        std::ofstream archivo(ruta.c_str(), std::ios::binary | std::ios::app);

        if (!archivo.is_open()) {
            return -2;
        }
        else {
            archivo.write(reinterpret_cast<const char*>(bytes.data()),bytes.size());
            archivo.close();

            return 1;
            }
    }
    catch (const std::exception& err) {
        return -1;
    }
}

/*
* Lee los últimos N bytes
*/
int gestor_arch::leer_final(const std::string ruta, CryptoPP::SecByteBlock& salida, const unsigned long tam) {
    try {
        /*
        * Se carga para ser leido
        */
        std::ifstream archivo(ruta.c_str(), std::ios::binary | std::ios::app);

        if (!archivo.is_open()) {
            return -2;
        }
        else {
            archivo.seekg(-static_cast<std::streamoff>(tam), std::ios::end);

            /*
            * Lectura y carga hacia el bloque seguro
            */
            archivo.read(reinterpret_cast<char*>(salida.data()), salida.size());

            archivo.close();

            return 1;
        }
    }
    catch (const std::exception& err) {
        return -1;
    }
}
