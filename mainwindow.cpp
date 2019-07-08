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
    //qDebug() << " recentfilelist count: " << RecentFilesList.count();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpenFile_triggered()
{
    QString scriptFileName =  QFileDialog::getOpenFileName(
              this,
              "Open Document",
              QDir::currentPath(),
              "Script files (*.gpc *.gph);;All files (*.*)");

    if( !scriptFileName.isNull() )
    {
      qDebug() << "selected file path : " << scriptFileName.toUtf8();
      readSource(scriptFileName);
      addRecentFile(scriptFileName);
    }
}

void MainWindow::on_actionReadClipboard_triggered()
{
    readSource("clipboard");
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
    qDebug() << "readRecentFiles() found recent files : " << count;
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

    qDebug() << "showRecentFiles() found recent files : " << RecentFilesList.count();

    if (RecentFilesList.count()<1) ui->menuRe_cent->menuAction()->setEnabled(false);
    else ui->menuRe_cent->menuAction()->setEnabled(true);

    int irfl=0;
    foreach (QAction *action, ui->menuRe_cent->actions()) {
         //qDebug() << "recent " << action->text() << " ... " << RecentFilesList.count();
         if (irfl < RecentFilesList.count()) {
            action->setText(QString::number(irfl) + ": " + RecentFilesList[irfl]);
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


void MainWindow::on_actionExit_triggered()
{
    close();
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About IC-Bytetable"),
             tr("IC-Bytetable\n"
                "Version: %1"
                "\n\nThe program is provided AS IS with NO WARRANTY OF ANY KIND \
                INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY, AND FITNESS \
                FOR A PARTICULAR PURPOSE.").arg(QCoreApplication::applicationVersion())
                );

}

void MainWindow::on_actionRecent_File1_triggered()
{
    QAction *a = qobject_cast<QAction*>(sender());
    if (a->isEnabled()) readSource(a->data().toString());
    addRecentFile(a->data().toString());
}


void MainWindow::on_actionRecent_File2_triggered()
{
    QAction *a = qobject_cast<QAction*>(sender());
    if (a->isEnabled()) readSource(a->data().toString());
    addRecentFile(a->data().toString());
}

void MainWindow::on_actionRecent_File3_triggered()
{
    QAction *a = qobject_cast<QAction*>(sender());
    if (a->isEnabled()) readSource(a->data().toString());
    addRecentFile(a->data().toString());
}

void MainWindow::on_actionRecent_File4_triggered()
{
    QAction *a = qobject_cast<QAction*>(sender());
    if (a->isEnabled()) readSource(a->data().toString());
    addRecentFile(a->data().toString());
}

void MainWindow::on_actionRecent_File5_triggered()
{
    QAction *a = qobject_cast<QAction*>(sender());
    if (a->isEnabled()) readSource(a->data().toString());
    addRecentFile(a->data().toString());
}

void MainWindow::on_actionRecent_File6_triggered()
{
    QAction *a = qobject_cast<QAction*>(sender());
    if (a->isEnabled()) readSource(a->data().toString());
    addRecentFile(a->data().toString());
}
