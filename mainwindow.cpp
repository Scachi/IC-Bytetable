#include <QApplication>
#include <QCommandLineParser>

#include <QtWidgets>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gpc.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readRecentFiles();
    modifyStatusBar(); // set resizing policy
}

MainWindow::~MainWindow()
{
    delete ui;
}

// reads initially selected file (by filedialog, commandline, clipboard-btn, reload-btn)
void MainWindow::readSource(QString sFilePath) {
    //ToDo: error checking ? file is found ? file can be read ?
    //ToDo: how to handle cliboard read #include files, GPCSelectedDir is emtpy then, ..where to search for ? ask the user to select directory or "skip" ?
    enableReloadBtn();
    showMessageStatusBar(sFilePath);
    GPCSelectedDir=QFileInfo(sFilePath).absolutePath().append("/");
    GPCSelectFilePath=sFilePath;
    gpc = new GPC(GPCSelectedDir,GPCSelectFilePath);
}

void MainWindow::enableReloadBtn() {
    if (!ui->actionReload->isEnabled()) ui->actionReload->setEnabled(true);
}

void MainWindow::modifyStatusBar() { // set resizing policy of statusBar
    StatusBarLabel = new QLabel;
    StatusBarLabel->setAlignment(Qt::AlignRight);
    statusBar()->addPermanentWidget(StatusBarLabel);
    statusBar()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
}

void MainWindow::showMessageStatusBar(QString msg) {
    StatusBarLabel->setText(msg);
}

static inline QString recentFilesKey() { return QStringLiteral("recentFileList"); }
static inline QString fileKey() { return QStringLiteral("file"); }

void MainWindow::readRecentFiles()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const int count = settings.beginReadArray(recentFilesKey());
    RecentFilesList.clear();
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        RecentFilesList.append(settings.value(fileKey()).toString());
    }
    settings.endArray();
    //qDebug() << "readRecentFiles() found recent files : " << count;
    showRecentFiles();
}

void MainWindow::writeRecentFiles()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,QCoreApplication::organizationName(), QCoreApplication::applicationName());
    //static void writeRecentFiles(const QStringList &files, QSettings &settings)
    const int count = RecentFilesList.count();
    settings.beginWriteArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        settings.setValue(fileKey(), RecentFilesList.at(i));
    }
    settings.endArray();
}

void MainWindow::addRecentFile(QString s) {
    RecentFilesList.prepend(s);
    RecentFilesList.removeDuplicates();
    if (RecentFilesList.count()>MaxRecentFiles) {
        RecentFilesList=RecentFilesList.mid(-1,MaxRecentFiles);
    }
    writeRecentFiles();
    showRecentFiles();
}

void MainWindow::showRecentFiles() {

    //qDebug() << "showRecentFiles() found recent files : " << RecentFilesList.count();

    if (RecentFilesList.count()<1) ui->menuRe_cent->menuAction()->setEnabled(false);
    else ui->menuRe_cent->menuAction()->setEnabled(true);

    int irfl=0;
    foreach (QAction *action, ui->menuRe_cent->actions()) {
         //qDebug() << "recent " << action->text() << " ... " << RecentFilesList.count();
         if (irfl < RecentFilesList.count()) {
            action->setText(QString::number(irfl+1) + ": " + RecentFilesList[irfl]);
            action->setData(RecentFilesList[irfl]);

            QFile inputFile(RecentFilesList[irfl]);
            action->setVisible(true);
            if (inputFile.exists()) action->setEnabled(true);
            else action->setEnabled(false);
         } else {
             action->setText("None");
             action->setVisible(false);
             action->setEnabled(false);
         }
         irfl++;
    }
}


void MainWindow::on_actionOpenFile_triggered()
{
    qDebug() << "GPCSelectedDir: " << GPCSelectedDir;
    QDir curPath=QDir::currentPath();
    if (GPCSelectedDir.length()>0) curPath.setPath(GPCSelectedDir);
    QString scriptFileName =  QFileDialog::getOpenFileName(
              this,
              "Open Document",
                curPath.path(),
              "Script files (*.gpc *.gph);;All files (*.*)");

    if( !scriptFileName.isNull() )
    {
      //qDebug() << "selected file path : " << scriptFileName.toUtf8();
      readSource(scriptFileName);
      addRecentFile(scriptFileName);
    }
}

void MainWindow::on_actionReadClipboard_triggered()
{
    readSource("clipboard");
}

void MainWindow::on_actionExit_triggered()
{
    close();
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msgBox;
        msgBox.setWindowIcon(QPixmap(":/resources/icon.ico"));
        msgBox.setWindowTitle(tr("About IC-Bytetable"));
        msgBox.setIconPixmap(QPixmap(":/resources/icon.ico"));
        msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
        //msgBox.setText("<a href='http://google.com/'>Google</a>");
        msgBox.setText(tr("IC-Bytetable, Version: %1<br><br>"
           "-= by %2 =-  <a href='https://github.com/Scachi'>https://github.com/Scachi</a><br>"
           "<br>A special shout out to consoletuner.com<br>"
           "<br>The program is provided AS IS with NO WARRANTY OF ANY KIND \
           INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY, AND FITNESS \
           FOR A PARTICULAR PURPOSE.").arg(QCoreApplication::applicationVersion()).arg(QCoreApplication::organizationName())
           );
        msgBox.setStandardButtons(QMessageBox::Close);
        msgBox.exec();
}

void MainWindow::addRecentFileTrigger(QAction *a) {
    if (a->isEnabled()) readSource(a->data().toString());
    addRecentFile(a->data().toString());
}


void MainWindow::on_actionRecent_File1_triggered()
{
    addRecentFileTrigger(qobject_cast<QAction*>(sender()));
}

void MainWindow::on_actionRecent_File2_triggered()
{
    addRecentFileTrigger(qobject_cast<QAction*>(sender()));
}

void MainWindow::on_actionRecent_File3_triggered()
{
    addRecentFileTrigger(qobject_cast<QAction*>(sender()));
}

void MainWindow::on_actionRecent_File4_triggered()
{
    addRecentFileTrigger(qobject_cast<QAction*>(sender()));
}

void MainWindow::on_actionRecent_File5_triggered()
{
    addRecentFileTrigger(qobject_cast<QAction*>(sender()));
}

void MainWindow::on_actionRecent_File6_triggered()
{
    addRecentFileTrigger(qobject_cast<QAction*>(sender()));
}

void MainWindow::on_actionReload_triggered()
{
    readSource(GPCSelectFilePath);
}
