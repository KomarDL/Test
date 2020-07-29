#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>
#include <QPointF>
#include <QDebug>

class DataProcessor : public QObject
{
    Q_OBJECT
    static constexpr quint32 dotsCount = 5000;
    static constexpr quint32 maxRawData = 50000;

public:
    static constexpr quint64 allTime = 0;
    static constexpr quint64 msecInSec = 1000;

    explicit DataProcessor(QObject *parent = nullptr);
signals:
    void finished();
    void error(QString);
    void dataReady(QList<QPointF>);
    void timeChanged(quint64 time);

public slots:
    void process();
    void stop()                             { emit finished(); };
    void setSmoothingParam(qreal param)     { lambda = param; qDebug() << "lambda changed"; }
    void reciveRawData(QList<QPointF> data);

protected:
    void customEvent(QEvent *event) override;

private:

    qreal lambda{1.};
    QList<QPointF> rawData;
    qint64 timeForVisualize{10 * msecInSec};

    QList<QPointF> selectData(const QList<QPointF>& data);
    QList<QPointF> shrinkToFit(const QList<QPointF>& data, const quint32 size);
    void smoothing(QList<QPointF>& data);
};

#endif // DATAPROCESSOR_H
