#include "gpswindow.h"
#include "ui_gpswindow.h"

gpsWindow::gpsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::gpsWindow)
{
    ui->setupUi(this);
}

gpsWindow::~gpsWindow()
{
    delete ui;
}
