#include "server.h"
#include <QNetworkInterface>
#include <QApplication>

Server::Server(const QHostAddress &address, quint16 port, QObject *parent):
    QObject(parent),
    serve(new QTcpServer(this))
{
    serve->listen(address, port);

    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            serverAddr = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (serverAddr.isNull())
        serverAddr = QHostAddress(QHostAddress::LocalHost).toString();

    connect(serve, &QTcpServer::newConnection, this, &Server::handleConnection);
}

Server::~Server()
{
    for(auto& socket : sockList) {
        socket->disconnectFromHost();
        socket->waitForDisconnected();
    }
}

void Server::sendData(qreal data)
{

    if (!sockList.isEmpty()){
        QByteArray serializedData;
        QDataStream serializer(&serializedData, QIODevice::WriteOnly);
        serializer.setVersion(QDataStream::Qt_5_15);
        serializer.setByteOrder(QDataStream::LittleEndian);
        serializer << data;
        for(auto& socket : sockList) {
            auto bytesWrited = 0;
            while (bytesWrited < serializedData.size()) {
                auto writed = socket->write(serializedData.data() + bytesWrited, serializedData.size() - bytesWrited);
                if (writed == -1) {
                    qDebug() << "send data error occured";
                    break;
                } else {
                    bytesWrited += writed;
                }

            }
        }
    }


}

void Server::process()
{
    //no special actions
}

void Server::handleConnection()
{
    sockList.push_back(serve->nextPendingConnection());
    connect(sockList.last(), &QAbstractSocket::errorOccurred, this, &Server::handleSocketError);
}

void Server::handleSocketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    auto senderSocket = dynamic_cast<QTcpSocket*>(sender());
    qDebug() << senderSocket->errorString();
    sockList.removeAt(sockList.indexOf(senderSocket));
}
