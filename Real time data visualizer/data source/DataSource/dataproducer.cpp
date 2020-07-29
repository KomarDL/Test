#include "dataproducer.h"
#include <QApplication>
#include <QDebug>
#include <QThread>
#include <QtMath>

constexpr auto periodWidth = 10000;
constexpr auto coef = 0.9;

DataProducer::DataProducer(AbstractGenerator *generator, QObject *parent) :
    QObject(parent),
    generator(generator)
{

}

void DataProducer::process()
{
    auto counter = 0;
    while (!isStop) {
        auto tmp = (coef * qSin(M_PI / periodWidth * (counter++ % (2*periodWidth)))) + (generator->generate() * coef);
        emit dataReady(tmp);
        qApp->processEvents();
    }
    emit finished();
}

void DataProducer::changeGenerator(AbstractGenerator *newGenerator)
{
    generator = newGenerator;
    qDebug() << "arrived ptr" << generator;
}
