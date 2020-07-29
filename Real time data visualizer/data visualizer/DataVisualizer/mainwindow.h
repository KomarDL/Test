#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QtCharts>
#include <QPointF>
#include "receiver.h"
#include "dataprocessor.h"

using namespace QtCharts;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QTcpSocket* socket, QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void stop();

public slots:
    void updateChart(QList<QPointF> data);
    void dataProcessorStoped();
    void receiverStoped();
    void receiverErrorOccured(QString err);
    void lambdaChanged(qreal lambda);
    void timeChanged(quint64 time);

private:
    Ui::MainWindow* ui;
    QDateTimeAxis* xAxis;
    QValueAxis* yAxis;
    QXYSeries* series;

    QThread* receiverThread;
    QThread* dataProcessorThread;

    Receiver* receiver;
    DataProcessor* dataProcessor;

    bool isProcessorStoped = false;
    bool isReceiverStoped = false;

    quint64 timeForVisualize = DataProcessor::allTime;

    void setupDataProcessor();
    void setupReceiver();
    void setupView(QChartView* view);
    void visualizationTime(quint64 ms) { timeForVisualize = ms; };
};

#endif // MAINWINDOW_H
