#ifndef LAMBDAEVENT_H
#define LAMBDAEVENT_H

#include <QEvent>

class LambdaEvent : public QEvent
{
public:
    LambdaEvent() : QEvent(LambdaEvent::type())
    {}

    static QEvent::Type type()
    {
        return eventType;
    }

    qreal lambda = 0.;
private:
    static const QEvent::Type eventType = static_cast<QEvent::Type>(QEvent::User + 1);
};

#endif // LAMBDAEVENT_H
