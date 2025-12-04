#include "app.h"
#include <gestor_arch.h>
#include "dialogs/confirm.h"
#include "dialogs/psswrd.h"
#include <memory>
#include "./ui_app.h"

app::app(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::app)
{
    ui->setupUi(this);
}

app::~app()
{
    delete ui;
}

//Simplificar creación de mensajes
void app::crearMsg(const QString& dato, const QString& titulo, const out_tipo& out,
    const QString& primer_boton, const QString& segun_boton) {
    confirm* vent = new confirm(this);
    vent->confg(dato,out, primer_boton,segun_boton);
    vent->setWindowModality(Qt::WindowModal);
    vent->setWindowTitle(titulo);
    vent->setWindowFlag(Qt::Window);
    vent->show();
}

void app::crearPsswrdVent() {
    if (copia.existe && !copia.ruta_arch.empty()) {
        psswrd* vent = new psswrd(this);
        //conexion para ejecutar derivacion
        QObject::connect(vent, &psswrd::out_password, &proc, &Proc_signal::get_clave);

        //conexion para arrojar menus de errores
        QObject::connect(&proc, &Proc_signal::crearMsg, this, &app::crearMsg);

        //paso intermedio para llamar 7zip
        QObject::connect(&proc, &Proc_signal::exitoso, this, &app::llamar_7zip);

        //corre 7zip
        QObject::connect(this, &app::run_7zip, &proc, &Proc_signal::llamar_7zip);

        vent->confg("accionar", "cancelar");
        vent->setWindowModality(Qt::WindowModal);
        vent->setWindowTitle("Ingresar Contraseña");
        vent->setWindowFlag(Qt::Window);
        vent->show();
    }
    else {
        crearMsg("Abre un archivo primero","Error",_ERROR,"Ok","");
    }
}

/*
* Sección relacionado con la barra de herramientas superior
*/
void app::on_actionOpen_triggered() {
    emit app::Opened_file(gestor_arch::explorador("Abrir archivo encriptado","Lock Files (*.lock)",OPEN));
}

void app::on_actionClose_triggered() {
    emit app::Close_file();
}

//Muestra un menu sobre nuestro programa
void app::on_actionabout_Pentaloon_triggered() {
    crearMsg("software Pentaloon","Pentaloon",_INFO , "Ok","");
}
//Lo mismo pero para las librerías de terceros
void app::on_actionabout_third_parties_triggered() {}

void app::actualizar_info(const meta_dat& datos) {
    ui->nombre_label->setText(QString(datos.nombre_arch.c_str()));
    ui->ruta_label->setText(QString(datos.ruta_arch.c_str()));
    ui->formato_label->setText(QString(datos.ext_arch.c_str()));
    ui->fecha_label->setText(QString(datos.fech_crea.c_str()));
    ui->tamano_label->setText(QString(std::to_string(float(datos.tam_bytes)/1000000).c_str()));
}

void app::on_encriptar_clicked()
{
    emit get_archivo();
    crearPsswrdVent();
}


void app::on_desencriptar_clicked()
{
    //emit get_archivo();
    //crearPsswrdVent();
}


void app::on_eliminar_clicked()
{
    confirm* vent = new confirm(this);
    vent->confg("¿Deseas eliminar el archivo?",ALERT,"Aceptar","Cancelar");
    vent->setWindowModality(Qt::WindowModal);

    //Conexion con borrar seguro
    QObject::connect(vent, &confirm::accepted, this, &app::borrar_arch);
    vent->setWindowTitle("Eliminar");
    vent->setWindowFlag(Qt::Window);
    vent->show();
}

void app::borrar_arch() {
    emit secure_era();
}

void app::setArchivo(const meta_dat& datos) {
    copia = datos;
}

// Asignar correctamente los datos para 7zip
void app::llamar_7zip(const datos_task &dato) {
    datos_task instruct;
    //resul_dt usado para almacenar ruta
    instruct.resul_dt = copia.ruta_arch;
    instruct.pro = 1; //1 Encripta y 2 desencripta
    //Se le pasa el hash como contraseña
    instruct.clave = std::get<CryptoPP::SecByteBlock>(dato.resul_dt);
    instruct.path = copia.ruta_arch;

    //Desconecta para evitar correr 2 veces
    QObject::disconnect(&proc, &Proc_signal::exitoso, this, &app::llamar_7zip);
    emit run_7zip(instruct);
}
