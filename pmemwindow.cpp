#include <qdebug.h>
#include <QCloseEvent>
#include <QtWidgets>

#include "pmemwindow.h"
#include "ui_pmemwindow.h"
#include "pmemmodel.h"
#include "pmem.h"
#include "pmemd.h"

class MainWindow;
#include "mainwindow.h"

PMEMWindow::PMEMWindow(QWidget *parent, class::MainWindow *mainwindow, QSettings *settings) :
    QDialog(parent),
    ui(new Ui::PMEMWindow)
{
    ui->setupUi(this);
    pmemModel = new PMEMModel;
    mainWindow = mainwindow;
    this->settings = settings;
}

PMEMWindow::~PMEMWindow()
{
    delete ui;
    settings = nullptr;
}

void PMEMWindow::updateModelData(PMEMD *pmemd)
{
    pmemModel->clear();
    pmemModel->pmemData=pmemd;
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

void PMEMWindow::scrollTo(QString byteoffset, QString bitsize, QString bitoffset)
{
    if (byteoffset.length()==0) return;
    int ibitsize = bitsize.toInt();
    QModelIndex index = pmemModel->index(byteoffset.toInt(),bitoffset.toInt());

    ui->tableView->setCurrentIndex(index);
    ui->tableView->selectionModel()->select(index,QItemSelectionModel::ClearAndSelect);

    for (int i=0; i<ibitsize; i++) {
        QModelIndex index = pmemModel->index(byteoffset.toInt()+((bitoffset.toInt()+i)/8),(bitoffset.toInt()+i)%8);
        /*
        qDebug() << "calc: " << byteoffset.toInt()+((bitoffset.toInt()+i)/8)
                 << " , " << (bitoffset.toInt()+i)%8;
        qDebug() << "index: " << index.row() << " , " << index.column();
        */
        ui->tableView->selectionModel()->select(index,QItemSelectionModel::Select);
    }
    ui->tableView->scrollTo(index,QAbstractItemView::PositionAtCenter);
}

void PMEMWindow::closeIt()
{
    this->close();
}

bool PMEMWindow::exportCSV(QString filename)
{
    return pmemModel->pmemData->exportCSV(filename);
}

void PMEMWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    if (settings != nullptr) settings->setValue("pmem/geometry", saveGeometry());
    this->hide();
}

void PMEMWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    PMEM pmem = pmemModel->pmemData->data[index.row()]; // access to data

    int iC = index.column();
    int iR = index.row();
    while (pmemModel->pmemData->data[iR].getTableMark(iC)=="b"
           || pmemModel->pmemData->data[iR].getTableMark(iC)=="x")
    {
        if (iC==0 ) {
            iR--;
            iC=7;
        }
        else iC--;
        if (iR<0) break;
    }
    mainWindow->scrollTo(iR, iC, pmem.getTableMark(iC));
}
