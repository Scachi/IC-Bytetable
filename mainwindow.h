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

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    enum { MaxRecentFiles = 5 };
    QStringList RecentFilesList;
    //QMenu* menu_File;
    //QMenu* menuRe_cent;



public:
    void readSource(QString s) {
        GPC *g = new GPC(s);
        qDebug() << "*g = new GPC(s) : " << s.toUtf8();
    }

private:
    void readRecentFiles();
    void writeRecentFiles();
    void addRecentFile(QString s);
    void showRecentFiles();
    //bool hasRecentFiles();
};

#endif // MAINWINDOW_H
