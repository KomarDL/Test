#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QTimer>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Video client");
    this->setFixedSize(this->size());
    setIPValidator();
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_connectButton_clicked()
{
    auto ipStr = ui->ipLineEdit->text();
    auto pos = 0;
    if (ui->ipLineEdit->validator()->validate(ipStr, pos) == QValidator::Acceptable) {
        initSockets(ipStr, ui->portSpinBox->value());
        if (!isError) {
            ui->infoLabel->setText("Connecting...");
            ui->connectButton->setEnabled(false);
            QTimer::singleShot(5 * 1000, this, SLOT(connectionFailed()));
        }
    } else {
        QMessageBox::information(this, "Ip error", "Incorrect ip address");
    }

}

void Dialog::socketConnected()
{
    isConnected = true;
    checkAndOpenMainWindow();
}

void Dialog::socketErrorOccured(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    isError = true;
    if (!isError) {
        QMessageBox::information(this, "Something went wrong", socket->errorString());
        socket->close();
        enableButton();
    }
}

void Dialog::connectionFailed()
{
    //if sockets didn't connect
    if (!isConnected) {
        // if no errors occured
        if (!isError) {
            socket->close();
            QMessageBox::information(this, "Connection error", "Unable connect to the server");
        }
        enableButton();
    }
}

void Dialog::enableButton()
{
    ui->infoLabel->clear();
    ui->connectButton->setEnabled(true);
}

void Dialog::setIPValidator()
{
    /* create regex for octet */
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";

    // create regex for ip
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    // create regex validator
    auto ipValidator = new QRegExpValidator(ipRegex, this);
    /* set validator for QLineEdit */
    ui->ipLineEdit->setValidator(ipValidator);
}

void Dialog::createSocket()
{
    if (socket != nullptr) {
        delete socket;
    }
    socket = new QTcpSocket;
}

void Dialog::initSockets(QString hostAddress, quint16 port)
{
    //recreate socket
    createSocket();

    connect(socket, &QAbstractSocket::connected, this, &Dialog::socketConnected);
    connect(socket, &QAbstractSocket::errorOccurred, this, &Dialog::socketErrorOccured);

    initFlags();
    // connect to the server
    socket->connectToHost(hostAddress, port);
}

void Dialog::initFlags()
{
    isError = isConnected = false;
}

void Dialog::checkAndOpenMainWindow()
{
    if (isConnected) {
        setHidden(true);
        auto mainWindow = new MainWindow(socket, this);
        mainWindow->show();
    }
}

