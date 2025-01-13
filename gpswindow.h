#ifndef GPSWINDOW_H
#define GPSWINDOW_H

#include <QMainWindow>

namespace Ui {
class gpsWindow;
}

class gpsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit gpsWindow(QWidget *parent = nullptr);
    ~gpsWindow();

private:
    Ui::gpsWindow *ui;
};

#endif // GPSWINDOW_H
