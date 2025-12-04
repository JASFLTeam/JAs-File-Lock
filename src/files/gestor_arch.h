#ifndef GESTOR_ARCH_H
#define GESTOR_ARCH_H
#include <fstream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>
#include <QObject>
#include <QFileDialog>
#include <QUrl>
#include <QByteArray>
#include "meta_dat.h"
#include "dialogs/confirm.h"

namespace fs = std::filesystem;

enum ex_op {
    OPEN = 1,
    SAVE = 2
};

class gestor_arch
{
public:

    gestor_arch() = default;

    meta_dat get_archivo(const std::string ruta);

    bool eliminar_seguro(const std::string ruta);

    int escribir_final(const std::string ruta, const CryptoPP::SecByteBlock& bytes);

    int leer_final(const std::string ruta, CryptoPP::SecByteBlock& salida, const unsigned long tam);

    static QString explorador(std::string titulo, std::string tipo, const ex_op op);

};

class Archivo : public QObject {

    Q_OBJECT

    public:
        Archivo();
        bool es_vacio() const;
        void limpiar();
        meta_dat obt_archivo() const;
        void guardar_arch(const meta_dat& dato);
    private:
        gestor_arch gestor;
        meta_dat archivo;

    signals:
        void salida(const meta_dat& datos);

        void crearMsg(const QString& dato, const QString& titulo, const out_tipo& out,
            const QString& primer_boton, const QString& segun_boton);

    public slots:
        void Abrir(const QString& url);
        void Borrar_s();
        void Cerrar();
        void ObtData();
};

#endif
