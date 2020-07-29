#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QRegExpValidator>
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_connectButton_clicked();

    void socketConnected();
    void socketErrorOccured(QAbstractSocket::SocketError socketError);

    void connectionFailed();
    void enableButton();

private:
    Ui::Dialog *ui;
    QTcpSocket* socket{};
    bool isError = false;
    bool isConnected = false;


    void setIPValidator();
    void createSocket();
    void initSockets(QString hostAddress, quint16 port);
    void initFlags();
    void checkAndOpenMainWindow();
};
#endif // DIALOG_H
