#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCharts>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <gpswindow.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gyroR(new QLineSeries())
    , gyroP(new QLineSeries())
    , gyroY(new QLineSeries())
    , accelR(new QLineSeries())
    , accelP(new QLineSeries())
    , accelY(new QLineSeries())
    , magP(new QLineSeries())
    , magR(new QLineSeries())
    , magY(new QLineSeries())
    , altitude(new QLineSeries())
    , temperature(new QLineSeries())
    , pressure(new QLineSeries())
    , voltage(new QLineSeries())
    , AutoGyroRotRate(new QLineSeries())
    , timer(new QTimer(this))
    , serialPort(new QSerialPort(this))
    , xValue(0)
{
    ui->setupUi(this);

    // GYRO_R, GYRO_P, GYRO_Y chart
    QChart *gyroChart = createChart3(gyroR, gyroP, gyroY, "Gyro");

    // ACCEL_R, ACCEL_P, ACCEL_Y chart
    QChart *accelChart = createChart3(accelR, accelP, accelY, "Acceleration");

    //MAG_R, MAG_P, MAG_Y chart
    QChart *magChart = createChart3(magR, magP, magY, "Magnetic");

    //ALTITUDE
    QChart *altitudeChart = createChart1(altitude, "Altitude: ");

    //TEMPERATURE
    QChart *tempChart = createChart1(temperature, "Temperature: ");

    //AUTO_GYRO_ROTATION_RATE

    QChart *AGRRChart = createChart1(AutoGyroRotRate, "AutoGyroRotRate: ");

    //PRESSURE
    QChart *pressChart = createChart1(pressure, "Pressure: ");

    //VOLTAGE
    QChart *voltChart = createChart1(voltage, "Voltage: ");

    // Layout ayarları
    QHBoxLayout *layout1 = new QHBoxLayout(ui->chartContainerTop);
    QHBoxLayout *layout2 = new QHBoxLayout(ui->chartContainerMid);
    QHBoxLayout *layout3 = new QHBoxLayout(ui->chartContainerBot);

    QChartView *chartView1 = new QChartView(gyroChart);
    chartView1->setRenderHint(QPainter::Antialiasing);
    layout1->addWidget(chartView1);

    QChartView *chartView2 = new QChartView(accelChart);
    chartView2->setRenderHint(QPainter::Antialiasing);
    layout1->addWidget(chartView2);

    QChartView *chartView3 = new QChartView(magChart);
    chartView3->setRenderHint(QPainter::Antialiasing);
    layout1->addWidget(chartView3);

    QChartView *chartView4 = new QChartView(altitudeChart);
    chartView3->setRenderHint(QPainter::Antialiasing);
    layout2->addWidget(chartView4);

    QChartView *chartView5 = new QChartView(tempChart);
    chartView3->setRenderHint(QPainter::Antialiasing);
    layout2->addWidget(chartView5);

    QChartView *chartView6 = new QChartView(AGRRChart);
    chartView3->setRenderHint(QPainter::Antialiasing);
    layout2->addWidget(chartView6);

    QChartView *chartView7 = new QChartView(pressChart);
    chartView3->setRenderHint(QPainter::Antialiasing);
    layout3->addWidget(chartView7);

    QChartView *chartView8 = new QChartView(voltChart);
    chartView3->setRenderHint(QPainter::Antialiasing);
    layout3->addWidget(chartView8);


    layout1->setContentsMargins(0, 0, 0, 0);
    layout2->setContentsMargins(0, 0, 0, 0);
    layout3->setContentsMargins(0, 0, 0, 0);
    ui->chartContainerTop->setLayout(layout1);
    ui->chartContainerMid->setLayout(layout2);
    ui->chartContainerBot->setLayout(layout3);

    // Seri port ayarları
    setupSerialPort();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (serialPort->isOpen()) {
        serialPort->close();
    }
}

QChart* MainWindow::createChart1(QLineSeries *series1, QString title1)
{
    series1->setName(title1);

    QChart *chart = new QChart();
    chart->addSeries(series1);

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Time (s)");
    axisX->setRange(0, 100);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText(title1);
    axisY->setRange(0, 100);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series1->attachAxis(axisX);
    series1->attachAxis(axisY);

    chart->setMargins(QMargins(0, 0, 0, 0));
    return chart;
}

QChart* MainWindow::createChart3(QLineSeries *series1, QLineSeries *series2, QLineSeries *series3,QString valueText)
{
    series1->setName("R: ");
    series2->setName("P: ");
    series3->setName("Y: ");

    QChart *chart = new QChart();
    chart->addSeries(series1);
    chart->addSeries(series2);
    chart->addSeries(series3);

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Time (s)");
    axisX->setRange(0, 100);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText(valueText);
    axisY->setRange(0, 100);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series1->attachAxis(axisX);
    series1->attachAxis(axisY);
    series2->attachAxis(axisX);
    series2->attachAxis(axisY);
    series3->attachAxis(axisX);
    series3->attachAxis(axisY);


    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setTitle(valueText);
    return chart;
}


void MainWindow::setupSerialPort()
{
    serialPort = new QSerialPort(this);

    qDebug() << "Number of Ports: " << QSerialPortInfo::availablePorts().length();
    foreach (auto &serialPortInfo, QSerialPortInfo::availablePorts()) {
        if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
            qDebug() << "VendorID: " << serialPortInfo.vendorIdentifier();
            qDebug() << "ProductID: " << serialPortInfo.productIdentifier();
            ui->cmbPort->addItem(serialPortInfo.portName());
        }
    }

    if (ui->cmbPort->count() > 0) {
        serialPort->setPortName(ui->cmbPort->currentText());
        serialPort->setBaudRate(QSerialPort::Baud9600);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);

        if (serialPort->open(QIODevice::ReadOnly)) {
            connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
            qDebug() << "Serial port opened successfully!";
        } else {
            QMessageBox::warning(this, "Port Error", "Couldn't open selected port.");
        }
    } else {
        QMessageBox::warning(this, "Port Error", "No available ports detected.");
    }
}

void MainWindow::readSerialData()
{
    while (serialPort->canReadLine()) {
        QByteArray line = serialPort->readLine().trimmed();
        QList<QByteArray> data = line.split(',');
        bool okA, okB;
        qreal valueA = data[2].toDouble(&okA);
        qreal valueB = data[0].toDouble(&okB);
       // if (okA && okB) {
            //updateChart(seriesA, seriesB, valueA, valueB);
       // }
        qDebug() << "Received data:" << line;
    }
}

//void MainWindow::updateChart(QLineSeries *series1, QLineSeries *series2, qreal valueA, qreal valueB)
//{
//    series1->append(xValue, valueA);
//    series2->append(xValue, valueB);
//    xValue++;

//    series1->setName(QString(": %1").arg(valueA, 0, 'f', 1));
//    series2->setName(QString(": %1").arg(valueB, 0, 'f', 1));

//    if (xValue > 20) {
//        series1->remove(0);
//        series2->remove(0);
//        static_cast<QValueAxis *>(seriesA->chart()->axes(Qt::Horizontal).first())->setRange(xValue - 20, xValue);
//    }
//}

void MainWindow::on_pushButton_clicked()
{
    gpsWindow g;
    g.show();
}

