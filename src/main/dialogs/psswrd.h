#ifndef PSSWRD_H
#define PSSWRD_H

#include <QDialog>

namespace Ui {
class psswrd;
}

class psswrd : public QDialog
{
    Q_OBJECT

public:
    explicit psswrd(QWidget *parent = nullptr);
    void confg(const QString& primer_boton, const QString& segundo_boton);
    ~psswrd();

private slots:
    void on_clave_textEdited(const QString &arg1);

    void on_buttonBox_accepted();

private:
    Ui::psswrd *ui;

signals:
    void out_password(const QString& psswrd);
};

#endif // PSSWRD_H
