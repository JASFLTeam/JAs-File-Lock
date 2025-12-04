#include "proc_signal.h"
#include "dialogs/confirm.h"
#include <logging.h>

Proc_signal::Proc_signal() {}

void Proc_signal::get_clave(const QString& clave) {
    datos = datos_task();
    QByteArray clave_bytes = clave.toUtf8();
    datos.resul_dt = CryptoPP::SecByteBlock((const unsigned char*)clave_bytes.constData(),
        clave_bytes.size());
    //Señal para iniciar e usar argon
    connect(this, &Proc_signal::usar_argon, &gestorTar, &ut::GestorTarea::ExecTarea);
    //Señal para recibir la salida generada
    connect(&gestorTar, &ut::GestorTarea::Exec_out, this, &Proc_signal::get_return);
    std::shared_ptr<ut::Tarea> tar = std::make_shared<task::UsarArgon2>();
    emit usar_argon(tar, datos);
}

void Proc_signal::get_return(const resul_task& result, const datos_task& dat) {
    datos = dat;
    resultado = result;

    /*
    * Nota: la variable clave en datos_task datos es usado para almacenar el salt
    * El hash checa resul_dt para tipo SecByteBlock
    */

    if (!resultado.exito) {
        std::string msg;
        if (resultado.mens_erro.has_value()) {
            msg = resultado.mens_erro.value();
            ut::Logger::Instancia().registrar(__FILE__,__LINE__,__FUNCTION__,msg,ERR);
        }
        else {
            msg = "Error desconocido";
            ut::Logger::Instancia().registrar(__FILE__,__LINE__,__FUNCTION__,msg,ERR);
        }
        emit crearMsg(QString::fromStdString(msg),"Error",_ERROR,"Ok", "");
    }
    else if (resultado.exito) {
        emit exitoso(datos);
    }
    emit done();
}

void Proc_signal::llamar_7zip(const datos_task& dat) {
    /*
    * Conexion para iniciar 7zip y desconectar para evitar doble llamada
    */
    disconnect(&gestorTar, &ut::GestorTarea::Exec_out, this, &Proc_signal::get_return);
    connect(this, &Proc_signal::usar_7zip, &gestorTar, &ut::GestorTarea::ExecTarea);
    //salida
    connect(&gestorTar, &ut::GestorTarea::Exec_out, this, &Proc_signal::get_return);
    std::shared_ptr<ut::Tarea> tar = std::make_shared<task::Usar7zip>();
    emit usar_7zip(tar, dat);
}
