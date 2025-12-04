#include "app.h"

#include "utils.h"
#include "logging.h"
#include <GestorTask.h>
#include <gestor_arch.h>

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#ifdef OS_WIN
    const std::string OS = "Build: windows";

#elifdef OS_LINUX
    const std::string OS = "Build: linux";
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*
    * Habilitar registro de resul_task y datos_task como meta-tipo de Qt
    */
    qRegisterMetaType<resul_task>("resul_task"); //el string es el nombre que vamos a usar para referenciar
    qRegisterMetaType<datos_task>("datos_task");
    qRegisterMetaType<meta_dat>("meta_dat");
    /*
    * Imprime el tipo de sistema orientado de nuestra build
    */
    ut::Logger::Instancia().registrar(__FILE__,__LINE__,__FUNCTION__,OS,INFO);

    /*
    * Instancia del archivo abierto
    */
    Archivo Active_archivo;
    /*
    * Instancia de Proc_signal
    */
    Proc_signal proc;

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "main_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    app w;

    /*
    * Conexiones
    */
    QObject::connect(&w, &app::Opened_file, &Active_archivo, &Archivo::Abrir);
    QObject::connect(&w, &app::Close_file, &Active_archivo, &Archivo::Cerrar);

    QObject::connect(&Active_archivo, &Archivo::salida, &w, &app::actualizar_info);

    /*
    * Conexiones del borrador seguro de archivos y los mensajes
    */
    QObject::connect(&w, &app::secure_era, &Active_archivo, &Archivo::Borrar_s);
    QObject::connect(&Active_archivo, &Archivo::crearMsg, &w, &app::crearMsg);
    //actualizar
    QObject::connect(&Active_archivo, &Archivo::salida, &w, &app::actualizar_info);

    //conexiones para usar 7zip seteando una copia de archivo activo
    QObject::connect(&w, &app::get_archivo, &Active_archivo, &Archivo::ObtData);
    QObject::connect(&Active_archivo, &Archivo::salida, &w, &app::setArchivo);

    w.show();
    return a.exec();
}
