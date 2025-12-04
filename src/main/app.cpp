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

    //Conectar una vez para la creacion de mensajes en ventana
    connect(&proc, &Proc_signal::crearMsg, this, &app::crearMsg);

    //Conectar actionabout_third_parties a page2
    connect(ui->actionabout_third_parties, &QAction::triggered,
            ui->stackedWidget, [=]() {
                ui->stackedWidget->setCurrentIndex(1);
            });

    connect(ui->Button, &QPushButton::clicked,
            ui->stackedWidget, [=]() {
                ui->stackedWidget->setCurrentIndex(0);
            });

    connect(ui->actionSobre_JAS_FileLock, &QAction::triggered,
            ui->stackedWidget, [=]() {
                ui->stackedWidget->setCurrentIndex(3);
            });

    connect(ui->Button_2, &QPushButton::clicked,
            ui->stackedWidget, [=]() {
                ui->stackedWidget->setCurrentIndex(0);
            });
    connect(ui->actionAyuda, &QAction::triggered,
            ui->stackedWidget, [=]() {
                ui->stackedWidget->setCurrentIndex(2);
            });
    connect(ui->Button_3, &QPushButton::clicked,
            ui->stackedWidget, [=]() {
                ui->stackedWidget->setCurrentIndex(0);
            });

    ui->toolBox->setItemText(
        0,
        " ¿Cual es el tamaño maximo que admite nuestro programa?"
        );
    ui->toolBox->setItemText(
        1,
        " ¿Para que sirve el boton rojo?"
        );
    ui->toolBox->setItemText(
        2,
        " ¿Para que sirve este software?"
        );
    ui->toolBox->setItemText(
        3,
        " ¿Es seguro?"
        );
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
    if ((copia.existe && !copia.ruta_arch.empty()) && copia.tam_bytes <= (1000 * 1000000)) {
        psswrd* vent = new psswrd(this);
        //conexion para ejecutar derivacion
        if (operacion == ENCRYPT) {
            //limpieza de conexiones
            disconnect(vent, &psswrd::out_password, this, &app::recuperar_clave);
            disconnect(this, &app::run_recu, &proc, &Proc_signal::bytes_to_clave);

            connect(vent, &psswrd::out_password, &proc, &Proc_signal::get_clave);
        }
        else if (operacion == DECRYPT) {
            //limpieza
            disconnect(vent, &psswrd::out_password, &proc, &Proc_signal::get_clave);

            connect(vent, &psswrd::out_password, this, &app::recuperar_clave);
            connect(this, &app::run_recu, &proc, &Proc_signal::bytes_to_clave);
        }

        //paso intermedio para llamar 7zip
        connect(&proc, &Proc_signal::exitoso, this, &app::llamar_7zip);

        //corre 7zip
        connect(this, &app::run_7zip, &proc, &Proc_signal::llamar_7zip);

        //desconecta la llamada hacia llamar_7zip para evitar doble llamada
        connect(&proc, &Proc_signal::disc_7zip, this, &app::disc_7zip);

        vent->confg("Aceptar", "Cancelar",copia);
        vent->setWindowModality(Qt::WindowModal);
        vent->setWindowTitle("Ingresar Contraseña");
        vent->setWindowFlag(Qt::Window);
        vent->show();
    }
    else if (copia.existe && (copia.tam_bytes > (1000 * 1000000))) {
        //setea máximo a 900mb como límite
        crearMsg("Maximo 900mb","Error",_ERROR,"Ok","");
    }
    else {
        crearMsg("Abre un archivo primero","Error",_ERROR,"Ok","");
    }
}

/*
* Sección relacionado con la barra de herramientas superior
*/
void app::on_actionOpen_triggered() {
    emit app::Opened_file(gestor_arch::explorador("Abrir archivo","Todos los archivos (*)",OPEN));
}

void app::on_actionClose_triggered() {
    emit app::Close_file();
}

//Lo mismo pero para las librerías de terceros
void app::on_actionabout_third_parties_triggered() {}

void app::actualizar_info(const meta_dat& datos) {
    ui->nombre_label->setText(QString(datos.nombre_arch.c_str()));
    ui->ruta_label->setText(QString(datos.ruta_arch.c_str()));
    ui->formato_label->setText(QString(datos.ext_arch.c_str()));
    ui->fecha_label->setText(QString(datos.fech_crea.c_str()));
    ui->tamano_label->setText(QString(std::to_string(float(datos.tam_bytes)/1000000).c_str()));

    /*
    * Actualizan los íconos correspondiente
    * dependiendo si es .lock o otra extensión
    */
    if (datos.ext_arch == ".lock") {
        ui->opened->setPixmap(QPixmap(":/basic/icons/lock_file_128px.svg").scaled(128, 128,
            Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (!datos.ext_arch.empty()) {
        ui->opened->setPixmap(QPixmap(":/basic/icons/file_128px.svg").scaled(128, 128,
            Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else {
        ui->opened->clear();
        ui->opened->setText("vacío");
    }
}

void app::on_encriptar_clicked()
{
    emit get_archivo();
    operacion = ENCRYPT;
    crearPsswrdVent();
}


void app::on_desencriptar_clicked()
{
    emit get_archivo();
    if (copia.ext_arch == ".lock") {
        operacion = DECRYPT;
        crearPsswrdVent();
    }
    else {
        crearMsg("No hay archivo encriptado abierto","Error",_ERROR,"Ok","");
    }
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
    if (operacion == ENCRYPT) {
        instruct.pro = ENCRYPT;
    }
    else if (operacion == DECRYPT) {
        instruct.pro = DECRYPT;
    }
    //Se le pasa el hash como contraseña
    instruct.clave = std::get<CryptoPP::SecByteBlock>(dato.resul_dt);
    instruct.path = copia.ruta_arch;
    //pasar salt
    if (dato.clave.has_value()) {
        instruct.sec_c = *dato.clave;
    }

    //Desconecta para evitar correr 2 veces
    QObject::disconnect(&proc, &Proc_signal::exitoso, this, &app::llamar_7zip);
    emit run_7zip(instruct);
}

/*
* Es un adaptador para llamar a bytes_to_clave
* Usado para recuperar salt del final del archivo
*/
void app::recuperar_clave(const QString& clave) {
    emit run_recu(clave,copia);
    disconnect(this, &app::run_recu, &proc, &Proc_signal::bytes_to_clave);
}

void app::disc_7zip() {
    disconnect(this, &app::run_7zip, &proc, &Proc_signal::llamar_7zip);
}
