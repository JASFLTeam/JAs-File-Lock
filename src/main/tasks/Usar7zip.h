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
            int escritura = 0;
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

                QString archivo = QString::fromStdString(path);

                QByteArray clave(reinterpret_cast<const char*>(datos_inicial.clave->data()),
                    datos_inicial.clave->size());

                //Transformar a base64
                QByteArray _clave = clave.toBase64();

                QString param_clave = "-p" + _clave;

                QString out = "-o";

                if (datos_inicial.pro == ENCRYPT) {

                    out_path.append(QString(".lock"));

                    ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,std::string("Encriptar: ")+archivo.toStdString(),INFO);
                    ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,std::string("Salida: ")+out_path.toStdString(),INFO);
                    params << "a" << "-mhe=on" << out_path << archivo << "-mx0" << param_clave; //Nota: -p es para ingresar contraseña
                }
                else if (datos_inicial.pro == DECRYPT) {

                    QString _out = QString::fromStdString(out_path.toStdString().erase((out_path.size()-nombre.size()),nombre.size()));
                    out = out + _out;

                    ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,std::string("Entrada: ")+out_path.toStdString(),INFO);
                    ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,std::string("Salida: ")+_out.toStdString(),INFO);

                    params << "e" << out_path << out << param_clave;
                }

                connect(proceso.get(), &QProcess::finished, this, &Usar7zip::fin);

                proceso->start(ejecutable,params);

                //limpiezas
                _clave.clear();
                param_clave.clear();
                clave.clear();

                /*
                * Ingreso de contraseña por stdin (como usando por consola)
                */
                if (proceso->waitForStarted(5000)) { // Espera por 5 segundo para que se inicie

                    proceso->waitForReadyRead(500);

                    //Remplaza si existe un archivo igual
                    proceso->write("y");

                    /*
                    * Cierra la entrada
                    */
                    proceso->closeWriteChannel();
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
                    ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,"Operacion con 7zip exitoso",INFO);
                    //escritura del salt al final
                    if (datos_inicial.pro == ENCRYPT) {
                        escritura = gestor.escribir_final(out_path.toStdString(), datos_inicial.sec_c);
                        if (proceso) {
                            result.exito = true;
                            ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,"Salt almacenado en el fichero",INFO);
                        }
                        else {
                            result.exito = false;
                            ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,"No se pudo almacenar el salt en el fichero",INFO);
                        }
                    }
                    else if (datos_inicial.pro == DECRYPT) {
                        result.exito = true;
                    }
                }
                else {
                    ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,"Error de 7zip",ERR);
                    result.codi_erro = exit_code;
                    result.mens_erro = "Error de 7zip o contraseña erronea";
                }
            }

        private slots:
            void fin(int code, QProcess::ExitStatus estado) {
                ut::Logger::Instancia().registrar(__FILE__, __LINE__,__FUNCTION__,std::string("Código de salida 7zip: ")+std::to_string(code),INFO);
                exit_code = code;
            }
    };
    
}
