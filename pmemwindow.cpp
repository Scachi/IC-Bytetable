#include <qdebug.h>
#include <QCloseEvent>
#include <QtWidgets>

#include "pmemwindow.h"
#include "ui_pmemwindow.h"
#include "pmemmodel.h"
#include "pmem.h"
#include "pmemd.h"

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

void PMEMWindow::setModel(PMEMModel *model)
{
    ui->tableView->setModel(nullptr);
    qDebug() << "set model:" << model;
    ui->tableView->setModel(model);
}

void PMEMWindow::updateStats(PMEMD *pmemd)
{
    int bytes=pmemd->getFreeBytes();
    int bits=pmemd->getFreeBits();
    ui->liFreeBytes->setText(QString::number(bytes));
    ui->liFreeBits->setText(QString::number(bits));
}

void PMEMWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("pmem/geometry", saveGeometry());
    this->hide();
}
