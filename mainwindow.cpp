#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gpc.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpenFile_triggered()
{
    QString scriptFileName =  QFileDialog::getOpenFileName(
              this,
              "Open Document",
              QDir::currentPath(),
              "Script files (*.gpc *.gph);;All files (*.*)");

    if( !scriptFileName.isNull() )
    {
      qDebug() << "selected file path : " << scriptFileName.toUtf8();
      Read(scriptFileName);
    }
}

void MainWindow::on_actionReadClipboard_triggered()
{
    Read("clipboard");
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

