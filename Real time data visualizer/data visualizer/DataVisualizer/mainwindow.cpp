#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QChartView>
#include "lambdaevent.h"
#include "timeevent.h"

MainWindow::MainWindow(QTcpSocket *socket, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    xAxis(new QDateTimeAxis),
    yAxis(new QValueAxis),
    series(new QLineSeries),
    receiverThread (new QThread),
    dataProcessorThread(new QThread),
    receiver(new Receiver(socket)),
    dataProcessor(new DataProcessor)
{
    ui->setupUi(this);
    auto view = new QChartView(this);
    ui->verticalLayout->addWidget(view);

    setupDataProcessor();
    setupReceiver();
    setupView(view);

    connect(receiver, &Receiver::dataReady, dataProcessor, &DataProcessor::reciveRawData);

    dataProcessorThread->start();
    receiverThread->start();

    ui->doubleSpinBox->setValue(1.);
    ui->spinBox->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateChart(QList<QPointF> data)
{
    series->replace(data);
    if (timeForVisualize == DataProcessor::allTime) {
        xAxis->setMin(QDateTime::fromMSecsSinceEpoch(data.first().x()));
    } else {
        xAxis->setMin(QDateTime::fromMSecsSinceEpoch(data.last().x() - timeForVisualize));
    }
    xAxis->setMax(QDateTime::fromMSecsSinceEpoch(data.last().x()));
}

void MainWindow::dataProcessorStoped()
{
    isProcessorStoped = true;
}

void MainWindow::receiverStoped()
{
    isReceiverStoped = true;
}

void MainWindow::receiverErrorOccured(QString err)
{
    QMessageBox::critical(this, "Ошибка получения данных", err);
}

void MainWindow::lambdaChanged(qreal lambda)
{
    LambdaEvent event;
    event.lambda = lambda;
    QApplication::sendEvent(dataProcessor, &event);
}

void MainWindow::timeChanged(quint64 time)
{
    TimeEvent event;
    event.time = time;
    QApplication::sendEvent(dataProcessor, &event);
}

void MainWindow::setupDataProcessor()
{
    dataProcessor->moveToThread(dataProcessorThread);
    //start worker when thread started
    connect(dataProcessorThread, &QThread::started, dataProcessor, &DataProcessor::process);
    //change smoothing param
    connect(ui->doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::lambdaChanged);
    //change time param
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::timeChanged);
    connect(dataProcessor, &DataProcessor::timeChanged, this, &MainWindow::visualizationTime);
    //notify that processor has been stopped
    connect(dataProcessorThread, &QThread::finished, this, &MainWindow::dataProcessorStoped);
    //delete objects when thread finished work
    connect(dataProcessorThread, &QThread::finished, dataProcessorThread, &QObject::deleteLater);
    connect(dataProcessor, &DataProcessor::finished, dataProcessor, &QObject::deleteLater);
    //exit thread event loop
    connect(dataProcessor, &DataProcessor::finished, dataProcessorThread, &QThread::quit);
    //receive and display processed data
    connect(dataProcessor, &DataProcessor::dataReady, this, &MainWindow::updateChart);

    connect(this, &MainWindow::stop, dataProcessor, &DataProcessor::stop);
}

void MainWindow::setupReceiver()
{
    receiver->moveToThread(receiverThread);
    //start worker when thread started
    connect(receiverThread, &QThread::started, receiver, &Receiver::process);
    //notify that producer has been stopped
    connect(receiverThread, &QThread::finished, this, &MainWindow::receiverStoped);
    //delete objects when thread finished work
    connect(receiverThread, &QThread::finished, receiverThread, &QObject::deleteLater);
    connect(receiver, &Receiver::finished, receiver, &QObject::deleteLater);
    //exit thread event loop
    connect(receiver, &Receiver::finished, receiverThread, &QThread::quit);
    //receive and display processed data
    connect(receiver, &Receiver::errorOccured, this, &MainWindow::receiverErrorOccured);

    connect(this, &MainWindow::stop, receiver, &Receiver::stop);
}

void MainWindow::setupView(QChartView *view)
{
    yAxis->setRange(-2, 2);
    xAxis->setFormat("hh:mm:ss:zzz");
    view->chart()->addSeries(series);
    view->chart()->addAxis(xAxis, Qt::AlignBottom);
    view->chart()->addAxis(yAxis, Qt::AlignLeft);
    view->chart()->legend()->hide();
    view->setRenderHint(QPainter::Antialiasing);
    series->attachAxis(xAxis);
    series->attachAxis(yAxis);
    series->setUseOpenGL(true);
}
