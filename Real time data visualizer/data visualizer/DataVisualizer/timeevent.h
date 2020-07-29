#ifndef TIMEEVENT_H
#define TIMEEVENT_H

#include <QEvent>

class TimeEvent : public QEvent
{
public:
    TimeEvent() : QEvent(TimeEvent::type())
    {}

    static QEvent::Type type()
    {
        return eventType;
    }

    quint64 time = 0;
private:
    static const QEvent::Type eventType = static_cast<QEvent::Type>(QEvent::User + 2);
};

#endif // TIMEEVENT_H
