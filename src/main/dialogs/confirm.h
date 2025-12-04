#ifndef CONFIRM_H
#define CONFIRM_H

#include <QDialog>

enum out_tipo {
    _ERROR,
    _INFO,
    ALERT
};

namespace Ui {
class confirm;
}

class confirm : public QDialog
{
    Q_OBJECT

public:
    explicit confirm(QWidget *parent = nullptr);
    void confg(const QString& dato, const out_tipo& out,
        const QString& primer_boton, const QString& segun_boton);
    ~confirm();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::confirm *ui;
};

#endif // CONFIRM_H
