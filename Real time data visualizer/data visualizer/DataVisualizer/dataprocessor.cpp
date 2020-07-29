#include "dataprocessor.h"
#include <algorithm>
#include <cmath>
#include <QDebug>
#include <QDateTime>
#include "lambdaevent.h"
#include "timeevent.h"

constexpr const quint32 DataProcessor::dotsCount;
constexpr quint64 DataProcessor::msecInSec;
constexpr quint32 DataProcessor::maxRawData;
constexpr quint64 DataProcessor::allTime;

DataProcessor::DataProcessor(QObject *parent) : QObject(parent)
{

}

void DataProcessor::process()
{
    //no special actions
}

void DataProcessor::reciveRawData(QList<QPointF> data)
{ 
    rawData.append(data);
    rawData = shrinkToFit(rawData, maxRawData * 3);
    auto resultData = selectData(rawData);
    resultData = shrinkToFit(resultData, dotsCount);
    smoothing(resultData);
    emit dataReady(resultData);
}

void DataProcessor::customEvent(QEvent *event)
{
    if (event->type() == LambdaEvent::type()) {
        auto lambdaEvent = static_cast<LambdaEvent*>(event);
        this->lambda = lambdaEvent->lambda;
    } else if (event->type() == TimeEvent::type()) {
        auto timeEvent = static_cast<TimeEvent*>(event);
        this->timeForVisualize = timeEvent->time * msecInSec;
        emit timeChanged(timeForVisualize);
    }
}

QList<QPointF> DataProcessor::selectData(const QList<QPointF>& data)
{
    QList<QPointF> result;
    if (timeForVisualize == allTime) {
        result = data;
    } else {
        auto lastTime = static_cast<qint64>(data.last().x());
        auto time = lastTime  - timeForVisualize;
        auto pos = std::find_if(data.begin(), data.end(),
                                [time](const QPointF& p1) { return static_cast<qint64>(p1.x()) >= time; });
        result = QList<QPointF>(pos, data.end());
    }
    return result;
}

QList<QPointF> DataProcessor::shrinkToFit(const QList<QPointF>& data, const quint32 size)
{
    if (static_cast<quint32>(data.size()) > size) {
        qreal step = static_cast<qreal>(data.size()) / size;
        QList<QPointF> result;
        result.reserve(size);
        for (auto i = 0. ; static_cast<int>(::trunc(i)) < data.size(); i += step ) {
            result.push_back(data.at(static_cast<int>(::trunc(i))));
        }
        result.push_back(data.last());;
        return result;
    }
    return data;
}

void DataProcessor::smoothing(QList<QPointF> &data)
{
    //exponential smoothing
    auto stPrev = data.first().y();
    for (auto item = ++data.begin(); item != data.end(); ++item) {
        //s[t] = s[t-1] + lambda * (x[t] - s[t-1])
        item->ry() = stPrev + lambda * (item->y() - stPrev);
        stPrev = item->y();
    }
}
