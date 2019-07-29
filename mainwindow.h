#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QLabel>

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

private:
    Ui::MainWindow *ui;
    enum { MaxRecentFiles = 6 };
    QStringList RecentFilesList;
    QString GPCSelectedDir;      // the path part of the file that was initially opened
    QString GPCSelectFilePath;   // selected filename from filedialog or recent file list
    QLabel *StatusBarLabel;

public:
    void readSource(QString sFilePath);
    void addRecentFile(QString s);
    void enableReloadBtn();
    void showMessageStatusBar(QString msg);
    void msgboxFileNotFound(QString sFilepath);
    void msgboxICNotFound(QString source);

private:
    void modifyStatusBar();
    void writeRecentFiles();
    void readRecentFiles();
    void addRecentFileTrigger(QAction *a);
    void showRecentFiles();
};

#endif // MAINWINDOW_H
