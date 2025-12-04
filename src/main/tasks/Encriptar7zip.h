#include <task.h>
#include <../crypt/argon_wrapper.h>
#include <QProcess>
#include <gestor_arch.h>
#include <QByteArray>
#include <logging.h>
#include <memory>
#include <logging.h>

namespace task {

    class Encriptar7zip : public ut::Tarea {
        private:
            /*
            * Usado para terminar
            */
            bool exit = false;
            int exit_code = 0;
            void ejecutar(const datos_task& datos_inicial,
                resul_task& result,
                datos_task& salida) override {

                /*
                * Gestion básica
                */
                gestor_arch gestor;

                std::string path = std::get<std::string>(datos_inicial.resul_dt);
                meta_dat dato = gestor.get_archivo(path);
                if (!dato.existe) {
                    ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,"Archivo no encontrado",WARN);
                    result.exito = false;
                    result.codi_erro = -1;
                    result.mens_erro = "El archivo no existe";
                    return;
                }

                std::unique_ptr<QProcess> proceso = std::make_unique<QProcess>();

                /*
                * Preparación de ejecución de 7zip
                * el nombre es pasado mediante path
                */

                QString ejecutable = "7z";
                QStringList params;
                QString nombre = "Nombre";
                QString archivo = QString::fromStdString(path);

                params << "a" << nombre << archivo << "-p"; //Nota: -p es para ingresar contraseña

                connect(proceso.get(), &QProcess::finished, this, &Encriptar7zip::fin);

                proceso->start(ejecutable,params);

                /*
                * Ingreso de contraseña por stdin (como usando por consola)
                */
                QByteArray clave(reinterpret_cast<const char*>(datos_inicial.clave->data()),
                    datos_inicial.clave->size());

                proceso->write(clave);
                proceso->write("\n");

                /*
                * Cierra la entrada y limpia el QByteArray clave
                */
                proceso->closeWriteChannel();
                clave.clear();

                /*
                * Espera hasta terminar
                */
                proceso->waitForFinished(-1);

                result.exito = exit_code;
                if (exit_code = 0) {
                    ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,"Error de 7zip",ERR);
                    result.mens_erro = "Error con 7zip";
                }
            }

        private slots:
            void fin(int code, QProcess::ExitStatus estado) {
                exit = true;
                if (code == 0) {
                    exit_code = 1;
                }
                else {
                    exit_code = 0;
                }
            }
    };
    
}
