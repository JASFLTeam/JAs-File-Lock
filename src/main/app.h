#ifndef APP_H
#define APP_H

#include <gestor_arch.h>
#include "dialogs/confirm.h"
#include "dialogs/psswrd.h"
#include <proc_signal.h>
#include <cryptopp/secblock.h>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class app;
}
QT_END_NAMESPACE

class app : public QMainWindow
{
    Q_OBJECT

public:
    app(QWidget *parent = nullptr);
    void crearMsg(const QString& dato, const QString& titulo, const out_tipo& out,
        const QString& primer_boton, const QString& segun_boton);
    void crearPsswrdVent();
    ~app();

private slots:
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionabout_Pentaloon_triggered();
    void on_actionabout_third_parties_triggered();

    void on_encriptar_clicked();

    void on_desencriptar_clicked();

    void on_eliminar_clicked();

public slots:
    void actualizar_info(const meta_dat& datos);
    void borrar_arch();
    void llamar_7zip(const datos_task& dato);
    void setArchivo(const meta_dat& datos);

signals:
    void Opened_file(const QString& url);
    void Close_file();

    void secure_era();

    void get_archivo();

    void run_7zip(const datos_task& dato);

private:
    meta_dat copia;
    Proc_signal proc;
    Ui::app *ui;
};
#endif // APP_H
