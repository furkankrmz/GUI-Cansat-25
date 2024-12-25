#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

private:
    Ui::MainWindow *ui;
    QLineSeries *seriesA; // Seri A
    QLineSeries *seriesB; // Seri B
    QTimer *timer;       // Zamanlayıcı
    int xValue;          // X ekseni değeri
    void updateChart();  // Grafik güncelleme fonksiyonu
};
#endif // MAINWINDOW_H
