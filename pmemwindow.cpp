#include <qdebug.h>
#include <QCloseEvent>
#include <QtWidgets>

#include "pmemwindow.h"
#include "ui_pmemwindow.h"

PMEMWindow::PMEMWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PMEMWindow)
{
    ui->setupUi(this);
}

PMEMWindow::~PMEMWindow()
{
    delete ui;
}


void PMEMWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("pmem/geometry", saveGeometry());
    this->hide();
}
