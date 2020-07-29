#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

class Server: public QObject
{
    Q_OBJECT
public:
    Server(const QHostAddress &address = QHostAddress::Any, quint16 port = 0, QObject* parent = nullptr);
    ~Server();

    QString address() const { return serverAddr; }
    quint16 port()    const { return serve->serverPort(); }

signals:
    void finished();

public slots:
    void sendData(qreal data);
    void process();
    void stop() { emit finished(); };

private slots:
    void handleConnection();
    void handleSocketError(QAbstractSocket::SocketError socketError);

private:
    QTcpServer* serve;
    QString serverAddr;
    QVector<QTcpSocket*> sockList;

    void serialize(qreal data);
};

#endif // SERVER_H
