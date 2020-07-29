#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <memory>
#include <QCloseEvent>
#include "exponentialgenerator.h"
#include "uniformgenerator.h"
#include "dataproducer.h"

#define GET_GENERATOR(index) static_cast<AbstractGenerator*>(ui->distributionBox->itemData(index).value<void*>())

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dataProducerThread(new QThread(this))
    , dataSenderThread(new QThread(this))
    , dataSender(new Server)
{
    ui->setupUi(this);

    setupBox();
    setupDataSender();
    setupDataProducer();

    ui->addressLbl->setText(tr("IP адрес: %1:%2").arg(dataSender->address()).arg(dataSender->port()));
    connect(ui->distributionBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeDistribution);

    dataProducerThread->start();
    dataSenderThread->start();
}

MainWindow::~MainWindow()
{
    auto tmp = ui->distributionBox->count();
    for (auto i = 0; i < tmp; ++i) {
        delete GET_GENERATOR(i);
    }

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit stop();
    ui->distributionBox->setEnabled(false);
    ui->addressLbl->setText("Отправляем последние полученные данные\n"
                            "Это может занять некоторое время\n"
                            "Программа закроется автоматически");
    waitForStop(isProducerStoped);
    waitForStop(isSenderStoped);
    event->accept();
}

void MainWindow::changeDistribution(int index)
{
    generator = GET_GENERATOR(index);
    qDebug() << "sended ptr" << generator;
    emit distributionChanged(generator);
}

void MainWindow::producerThreadStoped()
{
    isProducerStoped = true;
    qDebug() << "producer thread stopped";
}

void MainWindow::senderThreadStoped()
{
    isSenderStoped = true;
    qDebug() << "sender thread stopped";
}

void MainWindow::waitForStop(bool &flag)
{
    while (!flag) {
        qApp->processEvents();
    }
}

void MainWindow::setupBox()
{
    setBoxItem(new UniformGenerator);
    setBoxItem(new ExponentialGenerator);
}

void MainWindow::setBoxItem(AbstractGenerator *generator)
{
    ui->distributionBox->addItem(generator->distributionName(),
                                 QVariant::fromValue(static_cast<void*>(generator)));
}

void MainWindow::setupDataSender()
{
    dataSender->moveToThread(dataSenderThread);
    //start worker when thread started
    connect(dataSenderThread, &QThread::started, dataSender, &Server::process);
    //exit thread event loop
    connect(dataSender, &Server::finished, dataSenderThread, &QThread::quit);
    //notify that sender has been stopped
    connect(dataSenderThread, &QThread::finished, this, &MainWindow::senderThreadStoped);
    //delete objects when thread finished work
    connect(dataSenderThread, &QThread::finished, dataSender, &QObject::deleteLater);
    connect(dataSenderThread, &QThread::finished, dataSenderThread, &QObject::deleteLater);

    connect(this, &MainWindow::stop, dataSender, &Server::stop);
}

void MainWindow::setupDataProducer()
{
    auto index = ui->distributionBox->currentIndex();
    auto producer = new DataProducer(GET_GENERATOR(index));
    producer->moveToThread(dataProducerThread);
    //start worker when thread started
    connect(dataProducerThread, &QThread::started, producer, &DataProducer::process);
    //send data as soon as possible
    connect(producer, &DataProducer::dataReady, dataSender, &Server::sendData);
    //change random generator
    connect(this, &MainWindow::distributionChanged, producer, &DataProducer::changeGenerator);
    //notify that producer has been stopped
    connect(dataProducerThread, &QThread::finished, this, &MainWindow::producerThreadStoped);
    //delete objects when thread finished work
    connect(dataProducerThread, &QThread::finished, dataProducerThread, &QObject::deleteLater);
    connect(producer, &DataProducer::finished, producer, &QObject::deleteLater);
    //exit thread event loop
    connect(producer, &DataProducer::finished, dataProducerThread, &QThread::quit);

    connect(this, &MainWindow::stop, producer, &DataProducer::stop);
}
