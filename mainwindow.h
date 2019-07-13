#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QLabel>
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

    void on_actionRecent_File1_triggered();

    void on_actionRecent_File2_triggered();

    void on_actionRecent_File3_triggered();

    void on_actionRecent_File4_triggered();

    void on_actionRecent_File5_triggered();

    void on_actionRecent_File6_triggered();

    void on_actionReload_triggered();

private:
    Ui::MainWindow *ui;
    enum { MaxRecentFiles = 6 };
    QStringList RecentFilesList;
    QString GPCFilePath;
    QLabel *StatusBarLabel;


public:
    void readSource(QString s) {
        enableReloadBtn();
        showMessageStatusBar(s);
        GPCFilePath=s;
        GPC *g = new GPC(s);
        qDebug() << "*g = new GPC(s) : " << s.toUtf8();
    }
    void enableReloadBtn();
    void modifyStatusBar();
    void showMessageStatusBar(QString msg);


private:
    void writeRecentFiles();
    void readRecentFiles();
    void addRecentFile(QString s);
    void addRecentFileTrigger(QAction *a);
    void showRecentFiles();
};

#endif // MAINWINDOW_H
