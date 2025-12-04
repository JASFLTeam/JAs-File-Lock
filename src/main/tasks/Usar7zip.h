#include <task.h>
#include <../crypt/argon_wrapper.h>
#include <QProcess>
#include <gestor_arch.h>
#include <QByteArray>
#include <logging.h>
#include <thread>
#include <chrono>
#include <memory>
#include <logging.h>

enum crypto_op {
    ENCRYPT = 1,
    DECRYPT = 2
};

namespace task {

    class Usar7zip : public ut::Tarea {
        private:
            /*
            * Usado para terminar
            */
            int exit_code = 0;
            void ejecutar(const datos_task& datos_inicial,
                resul_task& result,
                datos_task& salida) override {

                /*
                * Gestion básica
                */
                gestor_arch gestor;

                //resul_dt es usado para almacenar la ruta
                const std::string& path = std::get<std::string>(datos_inicial.resul_dt);
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
                QString nombre = dato.nombre_arch.c_str();

                QString out_path = datos_inicial.path.c_str();
                QString out_param = "-o";

                out_param.append(out_path).append(QString(".lock"));

                QString archivo = QString::fromStdString(path);

                if (datos_inicial.pro == ENCRYPT) {
                    params << "a" << "-mhe=on" << nombre << out_param << archivo << "-mx0" << "-p"; //Nota: -p es para ingresar contraseña
                }
                else if (datos_inicial.pro == DECRYPT) {
                    params << "e" << nombre << out_param << "-p";
                }

                connect(proceso.get(), &QProcess::finished, this, &Usar7zip::fin);

                proceso->start(ejecutable,params);

                /*
                * Ingreso de contraseña por stdin (como usando por consola)
                */
                if (proceso->waitForStarted(5000)) { // Espera por 5 segundo para que se inicie
                    QByteArray clave(reinterpret_cast<const char*>(datos_inicial.clave->data()),
                        datos_inicial.clave->size());

                    proceso->write(clave);
                    proceso->write("\n");

                    /*
                    * Cierra la entrada y limpia el QByteArray clave
                    */
                    proceso->closeWriteChannel();
                    clave.clear();
                }
                else {
                    ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,"No se inicializó 7zip como es debido",ERR);
                    result.exito = false;
                    result.mens_erro = "7zip no se inició como es debido";
                    proceso->kill();
                    return;
                }

                /*
                * Espera hasta terminar
                */
                proceso->waitForFinished(-1);

                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                if (exit_code == 0) {
                    result.exito = true;
                    ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,"Operacion con 7zip exitoso",INFO);
                }
                else {
                    ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,"Error de 7zip",ERR);
                    result.codi_erro = exit_code;
                    result.mens_erro = "Error con 7zip";
                }
            }

        private slots:
            void fin(int code, QProcess::ExitStatus estado) {
                exit_code = code;
            }
    };
    
}
