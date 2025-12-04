#ifndef PSSWRD_H
#define PSSWRD_H

#include <QDialog>
#include <gestor_arch.h>
#include <QPushButton>

namespace Ui {
class psswrd;
}

class psswrd : public QDialog
{
    Q_OBJECT

public:
    explicit psswrd(QWidget *parent = nullptr);
    void confg(const QString& primer_boton, const QString& segundo_boton,const meta_dat& dat);
    ~psswrd();

    void actualizar();

private slots:
    void on_clave_textEdited(const QString &arg1);

    void on_buttonBox_accepted();

    void on_ver_contra_toggled(bool checked);

private:
    QString ruta;
    Ui::psswrd *ui;

signals:
    void out_password(const QString& psswrd);
};

#endif // PSSWRD_H
