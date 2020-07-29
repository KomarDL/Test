#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QTcpSocket>
#include <QPointF>

class Receiver : public QObject
{
    Q_OBJECT
public:
    explicit Receiver(QTcpSocket* socket, QObject *parent = nullptr);

signals:
    void finished();
    void errorOccured(QString);
    void dataReady(QList<QPointF>);

public slots:
    void process();
    void stop() { emit finished(); };

private slots:
    void socketError(QAbstractSocket::SocketError error);
    void receiveData();

private:
    QTcpSocket* socket;
};

#endif // RECEIVER_H
