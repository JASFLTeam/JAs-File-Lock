#include "app.h"

#include "utils.h"
#include "logging.h"
#include <GestorTask.h>

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#ifdef OS_WIN
    const std::string OS = "windows";

#elifdef OS_LINUX
    const std::string OS = "linux";
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*
    * Habilitar registro de resul_task y datos_task como meta-tipo de Qt
    */
    qRegisterMetaType<resul_task>("resul_task"); //el string es el nombre que vamos a usar para referenciar
    qRegisterMetaType<datos_task>("datos_task");
    /*
    * Test de logging
    */
    ut::Logger::Instancia().registrar(__FILE__,__LINE__,__FUNCTION__,OS,INFO);

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
    w.show();
    return a.exec();
}
