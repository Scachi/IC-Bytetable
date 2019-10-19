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

    pmemModel = new PMEMModel;
}

PMEMWindow::~PMEMWindow()
{
    delete ui;
}

void PMEMWindow::updateModelData(PMEMD *pmemd)
{
    pmemModel->pmemData.clear();
    pmemModel->pmemData.append(pmemd->data);
    ui->tableView->setModel(nullptr);
    ui->tableView->setModel(pmemModel);
}

void PMEMWindow::updateStats(PMEMD *pmemd)
{
    int bytes=pmemd->getFreeBytes();
    int bits=pmemd->getFreeBits();
    ui->liFreeBytes->setText(QString::number(bytes));
    ui->liFreeBits->setText(QString::number(bits));
    //qDebug() << "bytes: " << pmemd->getBytes() << " bits: " << pmemd->getBits();
}

void PMEMWindow::closeIt()
{
    this->close();
}

bool PMEMWindow::exportCSV(QString filename)
{
    return pmemModel->exportCSV(filename);
}

void PMEMWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("pmem/geometry", saveGeometry());
    this->hide();
}
