#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gpc.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpenFile_triggered();

    void on_actionExit_triggered();

    void on_actionReadClipboard_triggered();

private:
    Ui::MainWindow *ui;

public:
    void Read(QString s) {
        GPC *g = new GPC(s);
        qDebug() << "*g = new GPC(s) : " << s.toUtf8();
    }
};

#endif // MAINWINDOW_H
