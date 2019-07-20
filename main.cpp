#include <QApplication>
#include <QCommandLineParser>
#include <QMessageBox>
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Scachi");
    QCoreApplication::setApplicationName("IC-Bytetable");
    QCoreApplication::setApplicationVersion("1.0.0.0");

    MainWindow w;
    w.show();

    //ToDo: move the command line argument checking to somewhere else ?
    QCommandLineParser parser;
    parser.setApplicationDescription("Commandline Options");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("/path/to/scriptfile.gpc", QCoreApplication::translate("main", " The .gpc/.gph script file to read."));
    parser.process(a);
    const QStringList args = parser.positionalArguments();
    if (args.size()>0 && (args[0].endsWith(".gpc") || args[0].endsWith(".gph"))) w.readSource(args[0]);

    return a.exec();
}
