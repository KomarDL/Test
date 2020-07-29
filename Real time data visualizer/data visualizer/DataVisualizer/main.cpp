#include "dialog.h"

#include <QApplication>

#include "config.h"
#include <QAbstractSocket>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    qRegisterMetaType<QList<QPointF>>();
    qRegisterMetaType<QAbstractSocket::SocketError>();

    Dialog w;
    w.show();
    return a.exec();
}
