#ifndef PROC_SIGNAL_H
#define PROC_SIGNAL_H

#include <QObject>
#include <Interfaces/datos_task.h>
#include <Interfaces/resul_task.h>
#include <GestorTask.h>
#include <tasks/tasksLoad.h>
#include <cryptopp/secblock.h>
#include "dialogs/confirm.h"
#include <gestor_arch.h>
#include <memory>

class Proc_signal : public QObject
{
    Q_OBJECT

public:
    Proc_signal();

private:
    ut::GestorTarea gestorTar;
    gestor_arch gestor_arch;
    datos_task datos;
    resul_task resultado;

signals:
    void usar_argon(std::shared_ptr<ut::Tarea>&, const datos_task& dato);

    void crearMsg(const QString& dato, const QString& titulo, const out_tipo& out,
        const QString& primer_boton, const QString& segun_boton);

    void usar_7zip(std::shared_ptr<ut::Tarea>&, const datos_task& dato);

    void done();

    void exitoso(const datos_task& dat);

public slots:
    /*
    * Obtención y derivación de clave
    */
    void get_clave(const QString& clave);

    void llamar_7zip(const datos_task& dat);

    void get_return(const resul_task& result, const datos_task& dat);

};

#endif // PROC_SIGNAL_H
