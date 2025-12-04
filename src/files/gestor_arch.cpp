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
