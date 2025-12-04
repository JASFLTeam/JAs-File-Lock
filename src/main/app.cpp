#include "app.h"
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
