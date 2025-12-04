#include "gestor_arch.h"
#include "meta_dat.h"
#include <logging.h>
#include <ctime>
#include <chrono>

meta_dat gestor_arch::get_archivo(const std::string ruta) {
    meta_dat salida;
    salida.existe = fs::exists(ruta);
    if (salida.existe) {
        /*
        * Los metadatos
        * ruta, tamaño, nombre, extensión y fechas
        */
        salida.ruta_arch = ruta;
        salida.tam_bytes = fs::file_size(ruta);

        salida.nombre_arch = fs::path(ruta).filename().string();

        salida.ext_arch = fs::path(ruta).extension().string();
        
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

/*
* Abre un explorador que reconoce formatos específicos
* y retorna un string de su path
*/
QString gestor_arch::explorador(std::string titulo, std::string tipo, const ex_op op) {
    /*
    * Inicia el explorador
    */
    if (op == OPEN) {
        QUrl dir = QFileDialog::getOpenFileUrl(nullptr,
        titulo.c_str(), QUrl(), tipo.c_str());
        QString qs_dir = dir.path();
        return qs_dir;
    }
    else if (op == SAVE) {
        QUrl dir = QFileDialog::getSaveFileUrl(nullptr,
        titulo.c_str(), QUrl(), tipo.c_str());
        QString qs_dir = dir.path();
        return qs_dir;
    }

    return QString();
}

/*
* Series de funciones de Archivo
* para la gestión de un archivo usando sus metadatos
*/
Archivo::Archivo() {
    archivo.tam_bytes = 0;
}

bool Archivo::es_vacio() const {
    if (archivo.nombre_arch.empty() && archivo.ruta_arch.empty()) {
        return true;
    }
    else {
        return false;
    }
}
void Archivo::limpiar() {
    archivo.existe = false;
    archivo.ext_arch.clear();
    archivo.fech_crea.clear();
    archivo.nombre_arch.clear();
    archivo.ruta_arch.clear();
    archivo.tam_bytes = 0;
}
meta_dat Archivo::obt_archivo() const {
    return archivo;
}
void Archivo::guardar_arch(const meta_dat& dato) {
    archivo = dato;
}
void Archivo::Abrir(const QString& url) {
    limpiar();
    ut::Logger::Instancia().registrar(__FILE__,__LINE__,__FUNCTION__,url.toStdString().append(" Intentando abrir"),INFO);
    archivo = gestor.get_archivo(url.toStdString().erase(0,1));
    if (archivo.existe) {
        ut::Logger::Instancia().registrar(__FILE__,__LINE__,__FUNCTION__,url.toStdString().append(" Abierto"),INFO);
    }
    else {
        ut::Logger::Instancia().registrar(__FILE__,__LINE__,__FUNCTION__,url.toStdString().append(" No se pudo abrir"),INFO);
    }
    emit salida(archivo);
}
void Archivo::Cerrar() {
    limpiar();
    emit salida(archivo);
}
void Archivo::ObtData() {
    emit salida(archivo);
}

//Borrado seguro con sobreescritura
void Archivo::Borrar_s() {
    if (es_vacio()) {
        emit crearMsg("No existe ningún fichero","Error",_ERROR,"Ok","");
    }
    else if (archivo.tam_bytes > 1000000000LL) {
        emit crearMsg("Archivo demasiado grande","Error",_ERROR,"Ok","");
        ut::Logger::Instancia().registrar(__FILE__,__LINE__,__FUNCTION__,std::to_string(archivo.tam_bytes),INFO);
    }
    else if (!es_vacio() && archivo.tam_bytes < 1000000000LL) {
        bool proceso = gestor.eliminar_seguro(archivo.ruta_arch);
        if (proceso) {
            std::string msg = std::string("Eliminado: ") + archivo.nombre_arch;
            emit crearMsg(QString::fromStdString(msg),"Exito",_INFO,"Ok","");
            ut::Logger::Instancia().registrar(__FILE__,__LINE__,__FUNCTION__,msg,INFO);
        }
        else {
            emit crearMsg("No se pudo eliminar","Error",_ERROR,"Ok","");
        }
    }
    limpiar();
    emit salida(archivo);
}
