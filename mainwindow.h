#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QLabel>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>
#include <QtWidgets>

#include "ic.h"
#include "icmodel.h"
#include "icproxy.h"
#include "pmemwindow.h"
//#include "pmemmodel.h"

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

    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void on_Import_clicked();

    void on_Create_clicked();

    void on_actionStay_On_Top_triggered();

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

public:
    QSettings *settings;
    void readSource(QString sFilePath);
    void addRecentFile(QString s);
    void enableReloadBtn();
    void showMessageStatusBar(QString msg);
    void showMessageToolBar(QString msg);
    void msgboxFileNotFound(QString sFilepath);
    void msgboxICNotFound(QString source);
    void msgboxICNotFound();
    void msgboxCSVExport(bool data, bool chart);
    void msgboxProblemsFound();
    void msgboxSumSize(int entries, int usingpmem, int bytes, int bits);
    void onTop(int value);
    void scrollTo(int byteoffset, int bitoffset, QString tablemark);

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

    QPoint      tvCtxCursorLocation;
    QStringList *tvCtxCopyIdx;
    QStringList *tvCtxCopyData;
    QStringList *tvCtxCopyDataHex;

    void tableViewCreateCtxMenu();
    QMenu *tvCtxMenu;
    QAction *tvCtxFieldCopy;
    QAction *tvCtxSelCopy;
    QAction *tvCtxChkCopy;
    QAction *tvCtxPaste;
    QAction *tvCtxSelDelete;
    QAction *tvCtxChkDelete;
    QAction *tvCtxAllDelete;
    QAction *tvCtxSelCheck;
    QAction *tvCtxSelUncheck;
    QAction *tvCtxAllCheck;
    QAction *tvCtxAllUncheck;
    QAction *tvCtxSelSumSize;
    QAction *tvCtxChkSumSize;
    QAction *tvCtxExportCSV;

    void tableViewFieldCopy();
    void tableViewSelectedCopy();
    void tableViewCheckedCopy();
    void tableViewPaste();

    void tableViewSelectedDelete();
    void tableViewCheckedDelete();
    void tableViewAllDelete();

    void tableViewSelectedCheck();
    void tableViewSelectedUncheck();
    void tableViewAllCheck();
    void tableViewAllUncheck();

    void tableViewSelectedSumSize();
    void tableViewCheckedSumSize();
    void csvExport();

    void configstringCreateButtonMenu();
    QMenu *cfgstrBtnMenu;
    QAction *cfgstrBtnCreateShort;
    QAction *cfgstrBtnCreateFull;
    QAction *cfgstrBtnCreateBitoffset;

    void configstringCreateShort();
    void configstringCreateFull();
    void configstringCreateBits();

private:
    void modifyStatusBar();
    void modifyToolBar();
    void writeRecentFiles();
    void readRecentFiles();
    void addRecentFileTrigger(QAction *a);
    void showRecentFiles();
    void closeEvent(QCloseEvent *event);
    void saveStateDefault();
    void restoreStateDefault();
};

#endif // MAINWINDOW_H
