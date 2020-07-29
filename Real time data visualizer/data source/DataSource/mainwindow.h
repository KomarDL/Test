#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QThread>
#include "abstractgenerator.h"
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void distributionChanged(AbstractGenerator*);
    void stop();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void changeDistribution(int index);
    void producerThreadStoped();
    void senderThreadStoped();
    void waitForStop(bool& flag);

private:
    Ui::MainWindow *ui;
    AbstractGenerator *generator;
    QThread *dataProducerThread;
    QThread *dataSenderThread;
    Server *dataSender;

    bool isProducerStoped = false;
    bool isSenderStoped = false;

    void setupBox();
    void setBoxItem(AbstractGenerator* generator);
    void setupDataSender();
    void setupDataProducer();
};

#endif // MAINWINDOW_H
