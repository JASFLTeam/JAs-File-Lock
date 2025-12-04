#include "confirm.h"
#include <QPushButton>
#include "ui_confirm.h"

confirm::confirm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::confirm)
{
    ui->setupUi(this);
}

confirm::~confirm()
{
    delete ui;
}

//Ventana de confirmación genérica, con escalado
void confirm::confg(const QString& dato, const out_tipo& out,
                    const QString& primer_boton, const QString& segun_boton) {

    /*
    * La parte de los iconos
    */
    if (out == _ERROR) {
        //centraliza los botones
        ui->buttonBox->setCenterButtons(true);

        //oculta el segundo boton
        ui->buttonBox->button(QDialogButtonBox::Cancel)->hide();
        ui->ico->setPixmap(QPixmap(":/basic/icons/red-x-icon.svg").scaled(24, 24,
            Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (out == ALERT) {
        ui->ico->setPixmap(QPixmap(":/basic/icons/risk-icon.svg").scaled(24, 24,
            Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (out == _INFO) {
        ui->buttonBox->setCenterButtons(true);

        ui->buttonBox->button(QDialogButtonBox::Cancel)->hide();
        ui->ico->setPixmap(QPixmap(":/basic/icons/round-information-blue-icon.svg").scaled(24, 24,
            Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (out == APP) {
        ui->buttonBox->setCenterButtons(true);

        ui->buttonBox->button(QDialogButtonBox::Cancel)->hide();
        ui->ico->setPixmap(QPixmap(":/basic/icons/app.svg").scaled(64, 64,
            Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    ui->label->setText(dato);

    //los botones
    auto primer = ui->buttonBox->button(QDialogButtonBox::Ok);
    primer->setText(primer_boton);
    if (out != _ERROR && out != _INFO) {
        auto segundo = ui->buttonBox->button(QDialogButtonBox::Cancel);
        segundo->setText(segun_boton);
    }
}

void confirm::on_buttonBox_accepted()
{
}

