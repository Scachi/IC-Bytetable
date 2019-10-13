#ifndef PMEMWINDOW_H
#define PMEMWINDOW_H

#include <QDialog>

#include "pmemd.h"
#include "pmemmodel.h"

namespace Ui {
class PMEMWindow;
}

class PMEMWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PMEMWindow(QWidget *parent = nullptr);
    void setModel(PMEMModel *model);
    void updateModelData(PMEMD *pmemd);
    void updateStats(PMEMD *pmemd);
    void closeIt();
    ~PMEMWindow();

private:
    Ui::PMEMWindow *ui;
    PMEMModel *pmemModel;

    void closeEvent(QCloseEvent *event);

};

#endif // PMEMWINDOW_H
