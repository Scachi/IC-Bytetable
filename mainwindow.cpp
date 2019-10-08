#include <QApplication>
#include <QCommandLineParser>

#include <QtWidgets>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "gpcreader.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ic.h"
#include "icmodel.h"
#include "xtra.h"
#include "pmemwindow.h"
#include "pmemmodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,QCoreApplication::organizationName(), QCoreApplication::applicationName());
    restoreGeometry(settings.value("main/geometry").toByteArray());
    restoreState(settings.value("main/state").toByteArray());

    readRecentFiles();
    modifyStatusBar(); // set resizing policy
    modifyToolBar();
    icModel = new ICModel;
    icProxy = new ICProxy;

    pmemWindow = new PMEMWindow(this);
    Qt::WindowFlags flags(Qt::Dialog|Qt::WindowCloseButtonHint);
    pmemWindow->setWindowFlags(flags);
    pmemModel = new PMEMModel;
    //qDebug() << "model:" << pmemModel;
    pmemWindow->setModel(pmemModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// reads initially selected file (by filedialog, commandline, clipboard-btn, reload-btn)
void MainWindow::readSource(QString sFilePath) {

    sFilePath = sFilePath.replace("\\","/");
    if (QFileInfo(sFilePath).exists() ||
        sFilePath.compare("clipboard") == 0)
    {
            //ToDo: how to handle cliboard read #include files, GPCSelectedDir is the path of the program in this case, ..where to search for ? ask the user to select directory or "skip" ?
            enableReloadBtn();  // enable reload button
            showMessageStatusBar(sFilePath); // show filepath in statusbar
            gpcSelectedDir = QFileInfo(sFilePath).absolutePath().append("/"); // remember the scripts directory
            qDebug() << " ScriptsDir: " << gpcSelectedDir;
            qDebug() << " sFilePath: " << sFilePath;
            gpcSelectFilePath = sFilePath;
            addRecentFile(sFilePath); // add the file to the recent file list
            GPCReader *gpc = new GPCReader(gpcSelectedDir,gpcSelectFilePath); // read the content, parse the IC
            //gpc->icData->debug();
            if (!gpc->getGPCICFound()) // abort when no IC was found
            {
                msgboxICNotFound(gpcSelectFilePath);
                return;
            }

            // output
            // clear previous data
            icModel->clear();
            // fill the tableview
            icModel->icData.append(gpc->icData->data);
            //ui->tableView->setModel(&icModel);
            icProxy->setSourceModel(icModel);
            icProxy->setDynamicSortFilter(true);
            ui->tableView->setModel(icProxy);
            icProxy->invalidate();
            icProxy->colSortNow();
            QString tmp;
            tmp = tr("%1 Bits used by Interactive Configuration<br>Click for more information").arg(gpc->icData->bitsUsed);
            tmp=XTRA::xNoAutoLinebreaks(tmp);
            ui->actionPMEM_Usage->setToolTip(tmp);
            showMessageToolBar(tr("<br>%1 of 1024<br>bits used").arg(gpc->icData->bitsUsed));
            pmemModel->pmemData.clear();
            pmemModel->pmemData.append(gpc->pmemData->data);
            pmemWindow->setModel(pmemModel);
    } else {
        // file not found
        msgboxFileNotFound(sFilePath);
    }
}

void MainWindow::msgboxFileNotFound(QString sFilepath) {
    int ret = QMessageBox::warning(this, QCoreApplication::applicationName(),
                                   tr("File does not exists:\n"
                                      "  '%1' \n\n"
                                      "Do you want to select a file to open?").arg(sFilepath)
                                   ,
                                   QMessageBox::Yes | QMessageBox::Cancel,
                                   QMessageBox::Cancel);
    qDebug() << " Return value: " << ret;

    if (ret == QMessageBox::Yes) on_actionOpenFile_triggered();
}

void MainWindow::msgboxICNotFound(QString source) {
    QMessageBox::information(this, QCoreApplication::applicationName(),
                                   tr("No 'Interactive Configuration' found in:\n"
                                      "  '%1' ").arg(source)
                                   ,
                                   QMessageBox::Close);
}

void MainWindow::msgboxICNotFound() {
    QMessageBox::information(this, QCoreApplication::applicationName(),
                                   tr("No 'Interactive Configuration' found."
                                   ),
                                   QMessageBox::Close);
}


void MainWindow::enableReloadBtn() {
    if (!ui->actionReload->isEnabled()) ui->actionReload->setEnabled(true);
}

void MainWindow::modifyStatusBar() { // set resizing policy of statusBar
    statusBarLabel = new QLabel;
    statusBarLabel->setAlignment(Qt::AlignRight);
    statusBar()->addPermanentWidget(statusBarLabel);
    statusBar()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
}

void MainWindow::showMessageStatusBar(QString msg) {
    statusBarLabel->setText(msg);
}

void MainWindow::modifyToolBar() { // set resizing policy of statusBar
    toolBarLabel = new QLabel;
    toolBarLabel->setText("");
    toolBarLabel->setMargin(10);
    toolBarLabel->setAlignment(Qt::AlignCenter);
    ui->toolBar->addWidget(toolBarLabel);

    ui->toolBar->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
}

void MainWindow::showMessageToolBar(QString msg) {
    toolBarLabel->setText(msg);
}

static inline QString recentFilesKey() { return QStringLiteral("recentFileList"); }
static inline QString fileKey() { return QStringLiteral("file"); }

void MainWindow::readRecentFiles()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const int count = settings.beginReadArray(recentFilesKey());
    recentFilesList.clear();
    for (int i = 0; i < count; ++i)
    {
        settings.setArrayIndex(i);
        recentFilesList.append(settings.value(fileKey()).toString());
    }
    settings.endArray();
    //qDebug() << "readRecentFiles() found recent files : " << count;
    showRecentFiles();
}

void MainWindow::writeRecentFiles()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const int count = recentFilesList.count();
    settings.beginWriteArray(recentFilesKey());
    for (int i = 0; i < count; ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue(fileKey(), recentFilesList.at(i));
        qDebug() << " Recent file " << i << " : " << recentFilesList.at(i);
    }
    settings.endArray();
}

void MainWindow::addRecentFile(QString s) {
    //qDebug() << " addrecentfile qstring: " << s;
    if (s.compare("clipboard") == 0) return; // do no add clipboard to recent file list
    recentFilesList.prepend(s.replace("\\","/"));
    recentFilesList.removeDuplicates();
    //if (RecentFilesList.count()>MaxRecentFiles) qDebug() << "max recentlist reached ";
    if (recentFilesList.count() > MAX_RECENT_FILES)
    {
        recentFilesList = recentFilesList.mid(0,MAX_RECENT_FILES);
    }

    writeRecentFiles();
    showRecentFiles();
}

void MainWindow::showRecentFiles() {
    if (recentFilesList.count() < 1) ui->menuRe_cent->menuAction()->setEnabled(false);
    else ui->menuRe_cent->menuAction()->setEnabled(true);

    int irfl = 0;
    foreach (QAction *action, ui->menuRe_cent->actions())
    {
         //qDebug() << "recent " << action->text() << " ... " << RecentFilesList.count();
         if (irfl < recentFilesList.count())
         {
            action->setText(QString::number(irfl+1) + ": " + recentFilesList[irfl]);
            action->setData(recentFilesList[irfl]);

            QFile inputFile(recentFilesList[irfl]);
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
    qDebug() << "GPCSelectedDir: " << gpcSelectedDir;
    QDir curPath = QDir::currentPath();
    if (gpcSelectedDir.length() > 0) curPath.setPath(gpcSelectedDir);
    QString scriptFileName =  QFileDialog::getOpenFileName(
              this,
              "Open Document",
                curPath.path(),
              "Script files (*.gpc *.gph *.gcd);;All files (*.*)");

    if(!scriptFileName.isNull())
    {
      //qDebug() << "selected file path : " << scriptFileName.toUtf8();
      readSource(scriptFileName);
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
    readSource(gpcSelectFilePath);
}

void MainWindow::on_actionContextMenuToolbar_triggered()
{
    QPoint mp = ui->mainToolBar->mapFromGlobal(QCursor::pos());
    QAction *qa = ui->mainToolBar->actionAt(mp);
    if (qa == ui->actionOpenFile) ui->menuRe_cent->exec(QCursor::pos());
}

void MainWindow::on_actionPMEM_Usage_triggered()
{
    static bool first=true;
    static QByteArray mwGeometry = this->saveGeometry();
    QByteArray mwGeoNow = this->saveGeometry();
    if (mwGeoNow != mwGeometry) {
        mwGeometry = mwGeoNow;
        first=true;
    }
    if (first) {
        first=false;
        QScreen *screenactive=QGuiApplication::screenAt(this->pos());

        if (screenactive->geometry().width() - (this->geometry().x() + this->geometry().width()) >= pmemWindow->width())
        {
            pmemWindow->move(this->x()+this->width(),this->y());
        } else {
            pmemWindow->move(screenactive->geometry().width()-pmemWindow->width(),this->y());
        }
        pmemWindow->resize(pmemWindow->width(),this->height());

    }
    else if (pmemWindow->isHidden())
    {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope,QCoreApplication::organizationName(), QCoreApplication::applicationName());
        pmemWindow->restoreGeometry(settings.value("pmem/geometry").toByteArray());
    }
    pmemWindow->show();

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("main/geometry", saveGeometry());
    settings.setValue("main/state", saveState());
    QMainWindow::closeEvent(event);
}
