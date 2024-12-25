#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCharts>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , seriesA(new QLineSeries())
    , seriesB(new QLineSeries())
    , timer(new QTimer(this))
    , xValue(0)
{
    ui->setupUi(this);

    // Seri A ve Seri B için başlık ekle
    seriesA->setName("Sensor A: 0");
    seriesB->setName("Sensor B: 0");

    // QChart oluştur
    QChart *chart = new QChart();
    chart->addSeries(seriesA);
    chart->addSeries(seriesB);

    // X ve Y eksenleri oluştur
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Time (s)");  // X ekseni başlığı
    axisX->setRange(0, 20);          // X ekseni aralığı
    axisX->setLabelFormat("%d");     // Tam sayı formatı

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Value");    // Y ekseni başlığı
    axisY->setRange(0, 20);          // Y ekseni aralığı
    axisY->setLabelFormat("%.1f");   // Ondalık formatı

    // Ekseni grafiğe ekle ve serilere bağla
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    seriesA->attachAxis(axisX);
    seriesA->attachAxis(axisY);

    seriesB->attachAxis(axisX);
    seriesB->attachAxis(axisY);

    // Kenar boşluklarını kaldır
    chart->setMargins(QMargins(0, 0, 0, 0));

    // QChartView oluştur
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // chartView'ı UI'deki chartContainer widget'ına yerleştir
    QHBoxLayout *layout = new QHBoxLayout(ui->chartContainer);
    layout->addWidget(chartView);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    ui->chartContainer->setLayout(layout);

    // Zamanlayıcı ayarla
    connect(timer, &QTimer::timeout, this, &MainWindow::updateChart);
    timer->start(1000); // Her 500 ms'de bir çalışır
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateChart()
{
    // Sensor A için rastgele değer oluştur
    qreal newValueA = QRandomGenerator::global()->bounded(20);

    // Sensor B için rastgele değer oluştur
    qreal newValueB = QRandomGenerator::global()->bounded(20);

    // Seri A ve Seri B'ye yeni noktalar ekle
    seriesA->append(xValue, newValueA);
    seriesB->append(xValue, newValueB);
    xValue++;

    // Seri adlarını güncelle
    seriesA->setName(QString("Sensor A: %1").arg(newValueA, 0, 'f', 1));
    seriesB->setName(QString("Sensor B: %1").arg(newValueB, 0, 'f', 1));

    // X eksenini kaydır
    if (xValue > 20) {
        seriesA->remove(0); // Seri A'daki eski noktayı kaldır
        seriesB->remove(0); // Seri B'deki eski noktayı kaldır
        static_cast<QValueAxis *>(seriesA->chart()->axes(Qt::Horizontal).first())->setRange(xValue - 20, xValue);
    }
}
