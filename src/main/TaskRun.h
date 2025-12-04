#ifndef TASKRUN_H
#define TASKRUN_H
#include <task.h>
#include <memory>
#include <QRunnable>

namespace ut {
    class TareaExec : public QRunnable {
        public:
            TareaExec(std::shared_ptr<ut::Tarea> _tarea, const datos_task& _datos) : tarea(_tarea), datos(_datos) {
                setAutoDelete(true);
            }
            void run() override {
                (*tarea)(datos);
            }
        private:
            datos_task datos;
            std::shared_ptr<ut::Tarea> tarea;
    };
}

#endif
