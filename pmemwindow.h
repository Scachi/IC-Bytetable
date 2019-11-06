#ifndef PMEMWINDOW_H
#define PMEMWINDOW_H

#include <QDialog>
#include <QSettings>
#include <QTimer>

#include "pmemd.h"
#include "pmemmodel.h"

class MainWindow;

namespace Ui {
class PMEMWindow;
}

class PMEMWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PMEMWindow(QWidget *parent = nullptr, class::MainWindow *mainwindow = nullptr, QSettings *settings = nullptr);
    void setModel(PMEMModel *model);
    void updateModelData(PMEMD *pmemd);
    void updateStats(PMEMD *pmemd);
    void scrollTo(QString byteoffset, QString bitsize, QString bitoffset);
    void closeIt();
    bool exportCSV(QString filename);
    ~PMEMWindow();

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::PMEMWindow *ui;
    PMEMModel *pmemModel;
    QSettings *settings;
    MainWindow *mainWindow;

    void closeEvent(QCloseEvent *event);

};


#endif // PMEMWINDOW_H
