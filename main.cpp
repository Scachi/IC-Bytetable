#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Scachi");
    QCoreApplication::setApplicationName("IC-Bytetable");
    MainWindow w;
    w.show();

    return a.exec();
}
