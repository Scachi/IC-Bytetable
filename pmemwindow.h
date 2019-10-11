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
    void updateStats(PMEMD *pmemd);
    ~PMEMWindow();

private:
    Ui::PMEMWindow *ui;
    void closeEvent(QCloseEvent *event);

};

#endif // PMEMWINDOW_H
