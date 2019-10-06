#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QLabel>
#include <QSortFilterProxyModel>

#include "ic.h"
#include "icmodel.h"
#include "icproxy.h"
#include "pmemwindow.h"

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

    void on_actionContextMenuToolbar_triggered();

    void on_actionPMEM_Usage_triggered();

private:
    Ui::MainWindow *ui;

public:
    void readSource(QString sFilePath);
    void addRecentFile(QString s);
    void enableReloadBtn();
    void showMessageStatusBar(QString msg);
    void showMessageToolBar(QString msg);
    void msgboxFileNotFound(QString sFilepath);
    void msgboxICNotFound(QString source);
    void msgboxICNotFound();
    //QSortFilterProxyModel proxyModel;// sorting of tableview via proxyModel
    //ICPROXY proxyModel;// sorting of tableview via proxyModel

private:
    PMEMWindow *pmemWindow;
    enum { MAX_RECENT_FILES = 6 };
    QStringList recentFilesList;
    QString     gpcSelectedDir;     // the path part of the file that was initially opened
    QString     gpcSelectFilePath;  // selected filename from filedialog or recent file list
    QLabel      *statusBarLabel;
    QLabel      *toolBarLabel;
    ICModel     *icModel;            // tableview model
    ICProxy     *icProxy;            // QSortFilterProxyModel for sorting

private:
    void modifyStatusBar();
    void modifyToolBar();
    void writeRecentFiles();
    void readRecentFiles();
    void addRecentFileTrigger(QAction *a);
    void showRecentFiles();
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
