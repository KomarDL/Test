#include "receiver.h"
#include <QDateTime>
#include <QDataStream>

Receiver::Receiver(QTcpSocket *socket, QObject *parent):
    QObject(parent),
    socket(socket)
{
    socket->setParent(this);
}

void Receiver::process()
{
    qDebug() << "receiver started";
    connect(socket, &QAbstractSocket::errorOccurred, this, &Receiver::socketError);
    connect(socket, &QAbstractSocket::readyRead, this, &Receiver::receiveData);
}

void Receiver::socketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    emit errorOccured(socket->errorString());
    socket->disconnectFromHost();
    emit finished();
}

void Receiver::receiveData()
{

    if (socket->bytesAvailable() >= sizeof(qreal)) {
        QList<QPointF> list;

        while (socket->bytesAvailable() >= sizeof(qreal)) {
            QByteArray arr(sizeof (qreal), 0);
            QDataStream deserializer(&arr, QIODevice::ReadOnly);
            deserializer.setVersion(QDataStream::Qt_5_15);
            deserializer.setByteOrder(QDataStream::LittleEndian);
            auto bytesRead = 0u;
            while (bytesRead < sizeof(qreal)) {
                auto read = socket->read(arr.data() + bytesRead, sizeof(qreal) - bytesRead);
                if (read == -1) {
                    qDebug() << "read data error occured";
                    break;
                } else {
                    bytesRead += read;
                }
            }
            QPointF raw;
            deserializer >> raw.ry();
            raw.setX(QDateTime::currentMSecsSinceEpoch());
            list.append(raw);
        }
        emit dataReady(list);
    }

}
