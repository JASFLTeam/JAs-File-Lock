#ifndef TASK_H
#define TASK_H
#include <QObject>
#include <logging.h>
#include <Interfaces/datos_task.h>
#include <Interfaces/resul_task.h>

namespace ut {
    class Tarea : public QObject {

        Q_OBJECT

        private:

            virtual void ejecutar(
                const datos_task& datos_inicial,
                resul_task& result,
                datos_task& salida) = 0;
            /*
            * Lógica de la tarea
            */
        public:
            Tarea(QObject *parent = nullptr);

            Tarea(const Tarea&) = delete;

            /*
            * Funtor que acepta parametros para pasar a ejecutar()
            */
            void operator()(const datos_task& datos_inicial) {
                /*
                * Struct resultado de resul_task.h
                * y datos para retorno
                */
                resul_task resultado;
                datos_task datos_return;

                try{
                    ejecutar(datos_inicial, resultado, datos_return);
                    /*
                    * Se notifica una vez terminada
                    */
                    emit Exec_out(resultado, datos_return);
                }
                catch(std::exception& err) {
                    /*
                    * Se usa para notificar, en este caso errores excepcionales
                    * imposible de captar por la función ejecutar().
                    */
                    resultado.codi_erro = -1;
                    resultado.mens_erro = err.what();
                    ut::Logger::Instancia().registrar(__FILE__,__LINE__,__FUNCTION__,err.what(),ERR);
                    emit Exec_out(resultado, datos_return);
                }
            }
        signals:
            void Exec_out(const resul_task& result, const datos_task& datos);
    };
}

#endif
