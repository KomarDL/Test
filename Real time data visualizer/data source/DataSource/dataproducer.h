#ifndef DATAPRODUCER_H
#define DATAPRODUCER_H

#include <QObject>
#include "abstractgenerator.h"

class DataProducer : public QObject
{
    Q_OBJECT
public:
    explicit DataProducer(AbstractGenerator* generator, QObject *parent = nullptr);

signals:
    void finished();
    void dataReady(qreal);

public slots:
    void process();
    void stop() { isStop = true; };
    void changeGenerator(AbstractGenerator* newGenerator);

private:
    bool isStop = false;
    AbstractGenerator* generator;

};

#endif // DATAPRODUCER_H
