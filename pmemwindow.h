#ifndef PMEMWINDOW_H
#define PMEMWINDOW_H

#include <QDialog>

namespace Ui {
class PMEMWindow;
}

class PMEMWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PMEMWindow(QWidget *parent = nullptr);
    ~PMEMWindow();

private:
    Ui::PMEMWindow *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // PMEMWINDOW_H
