#include "app.h"

#include "utils.h"
#include "logging.h"

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
    * Test de logging
    */
    ut::Logger::Instancia().registrar(OS,INFO);

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
