#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineSeries>
#include <QChart>
#include <QTimer>
#include <QSerialPort>
#include <QValueAxis>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


    class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Grafik serileri
    QLineSeries *gyroR;
    QLineSeries *gyroP;
    QLineSeries *gyroY;

    QLineSeries *accelR;
    QLineSeries *accelP;
    QLineSeries *accelY;

    QLineSeries *magR;
    QLineSeries *magP;
    QLineSeries *magY;

    QLineSeries *altitude;
    QLineSeries *temperature;
    QLineSeries *pressure;
    QLineSeries *voltage;

    QLineSeries *AutoGyroRotRate;



    // Zamanlayıcı
    QTimer *timer;

    // Seri port
    QSerialPort *serialPort;

    // Grafik güncelleme için x ekseni değeri
    qreal xValue;

    // Yardımcı metotlar
    void setupSerialPort();
    void updateChart(QLineSeries *series1, QLineSeries *series2, qreal valueA, qreal valueB);

    QChart* createChart1(QLineSeries *series1, QString title1);
    QChart* createChart3(QLineSeries *series1, QLineSeries *series2, QLineSeries *series3,QString valueText);


private slots:
    void readSerialData();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
