#ifndef GESTORTASK_H
#define GESTORTASK_H
#include <QThreadPool>
#include <TaskRun.h>

namespace ut {
    class GestorTarea : public QObject {

        Q_OBJECT

        private:
            /*
            * Pozo de hilos global
            */
            QThreadPool* pozo = QThreadPool::globalInstance();

        public:
            GestorTarea() {
            }
        public slots:
            void ExecTarea(
                std::shared_ptr<ut::Tarea>& tarea,
                const datos_task& datos) {

                    /*
                    * Constructor de la nueva TareaRun
                    */
                    ut::TareaExec* Tarea = new TareaExec(tarea, datos);
                    /*
                    * Conexion entre la Tarea y el gestor
                    */
                    connect(tarea.get(), &ut::Tarea::Exec_out, this, [this](const resul_task& result, const datos_task& datos_out){
                        emit Exec_out(result, datos_out);
                    });
                    /*
                    * LLamada de ejecución
                    */
                    pozo->start(Tarea);
                }
        signals:
            void Exec_out(const resul_task& result, const datos_task& dat);
    };
}

#endif
