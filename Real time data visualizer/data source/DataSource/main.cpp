#include "mainwindow.h"

#include <QApplication>
#include "uniformgenerator.h"
#include "exponentialgenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<AbstractGenerator*>();
    qRegisterMetaType<UniformGenerator*>();
    qRegisterMetaType<ExponentialGenerator*>();

    MainWindow w;
    w.show();
    return a.exec();
}
