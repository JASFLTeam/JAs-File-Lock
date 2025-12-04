#include "psswrd.h"
#include "ui_psswrd.h"

psswrd::psswrd(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::psswrd)
{
    ui->setupUi(this);
}

psswrd::~psswrd()
{
    delete ui;
}

//crear ventana para obtener contraseña
void psswrd::confg(const QString& primer_boton, const QString& segundo_boton, const meta_dat& dat) {

    size_t long_t = dat.ruta_arch.size();
    std::string temp_r = dat.ruta_arch;
    ruta = QString::fromStdString(temp_r.erase(long_t-dat.nombre_arch.size(),dat.nombre_arch.size()));
    ui->url->setText(ruta);

    auto primer = ui->buttonBox->button(QDialogButtonBox::Ok);
    primer->setText(primer_boton);
    auto segundo = ui->buttonBox->button(QDialogButtonBox::Cancel);
    segundo->setText(segundo_boton);

}

void psswrd::actualizar() {
    ui->url->setText(ruta);
}

void psswrd::on_clave_textEdited(const QString &arg1)
{
    QString blanco("background-color: #ffffff;"
            "border: 1px solid #252525;"
            "border-radius: 4px;");
    if (arg1.length() <= 8 && arg1.length() != 0) {
        ui->nivel_1->setStyleSheet(
            "background-color: rgb(255, 169, 70);"
            "border: 1px solid #252525;"
            "border-radius: 4px;");
        ui->nivel_2->setStyleSheet(blanco);
        ui->nivel_3->setStyleSheet(blanco);
    }
    else if (arg1.length() > 8 && arg1.length() <= 16) {
        ui->nivel_2->setStyleSheet(
            "background-color: rgb(31, 144, 0);"
            "border: 1px solid #252525;"
            "border-radius: 4px;");
        ui->nivel_3->setStyleSheet(blanco);
    }
    else if (arg1.length() > 16 && arg1.length() != 0) {
        ui->nivel_3->setStyleSheet(
            "background-color: rgb(0, 106, 172);"
            "border: 1px solid #252525;"
            "border-radius: 4px;");
    }
    else if (arg1.length() == 0) {
        ui->nivel_1->setStyleSheet(blanco);
        ui->nivel_2->setStyleSheet(blanco);
        ui->nivel_3->setStyleSheet(blanco);
    }
}


void psswrd::on_buttonBox_accepted()
{
    emit out_password(ui->clave->text());
}


void psswrd::on_ver_contra_toggled(bool checked)
{
    if (checked) {
        ui->clave->setEchoMode(QLineEdit::Normal);
    }
    else if (!checked) {
        ui->clave->setEchoMode(QLineEdit::Password);
    }
}

