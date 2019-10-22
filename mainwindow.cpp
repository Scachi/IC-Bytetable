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
    tableViewCreateCtxMenu();
    icModel = new ICModel;
    icProxy = new ICProxy;

    tvCtxCopyIdx = new QStringList;
    tvCtxCopyData = new QStringList;
    tvCtxCopyDataHex = new QStringList;

    pmemWindow = new PMEMWindow(this); //<- single window shown in taskbar
    //pmemWindow = new PMEMWindow(); //<- separate windows shown in taskbar
    Qt::WindowFlags flags(Qt::Dialog|Qt::WindowCloseButtonHint);
    pmemWindow->setWindowFlags(flags);
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
            //qDebug() << " ScriptsDir: " << gpcSelectedDir;
            //qDebug() << " sFilePath: " << sFilePath;
            gpcSelectFilePath = sFilePath;
            addRecentFile(sFilePath); // add the file to the recent file list
            GPCReader *gpc = new GPCReader(gpcSelectedDir,gpcSelectFilePath); // read the content, parse the IC
            // clear previous data
            icModel->clear();
            //pmemWindow->updateModelData(gpc->pmemData);

            if (!gpc->getGPCICFound()) // abort when no IC was found
            {
                msgboxICNotFound(gpcSelectFilePath);
                return;
            }

            // output
            // fill the tableview
            icModel->icData=gpc->icData;
            //ui->tableView->setModel(&icModel);
            icProxy->setSourceModel(icModel);
            icProxy->setDynamicSortFilter(true);
            ui->tableView->setModel(icProxy);
            icProxy->invalidate();
            icProxy->colSortNow();
            ui->tableView->resizeColumnsToContents();
            ui->tableView->setColumnWidth(0,100);
            ui->tableView->setColumnWidth(8,100);
            ui->tableView->setColumnWidth(19,100);
            QString tmp;
            tmp = tr("%1 Bits used by Interactive Configuration<br>Click for more information").arg(gpc->icData->bitsUsed);
            tmp=XTRA::xNoAutoLinebreaks(tmp);
            ui->actionPMEM_Usage->setToolTip(tmp);
            showMessageToolBar(tr("%1 of 1024 bits used<br>( %2 bytes + %3 bits )").arg(gpc->icData->bitsUsed).arg(gpc->pmemData->getBytes()).arg(gpc->pmemData->getBits()));
            pmemWindow->updateModelData(gpc->pmemData);
            pmemWindow->updateStats(gpc->pmemData);

            if (!gpc->isValid() || !gpc->icData->isValid()) msgboxProblemsFound();

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
    //qDebug() << " Return value: " << ret;

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

void MainWindow::msgboxProblemsFound() {
    QMessageBox::warning(this, QCoreApplication::applicationName(),
                                   tr("Problems Found !\n\nCheck the lists 'Status' and 'Info' columns for details.\nEntries with problems may not be tracked in PMEM list."
                                   ),
                                   QMessageBox::Close);
}

void MainWindow::msgboxCSVExport(bool data, bool chart) {
    QString dataStatus=tr("OK");
    QString chartStatus=tr("OK");
    if (!data) dataStatus=tr("ERRROR");
    if (!chart) chartStatus=tr("ERROR");
    QString msg = tr("Data Export: %1\n"
                     "Chart Export: %2"
                  ).arg(dataStatus).arg(chartStatus);

    if (!data || !chart) {
        msg.prepend(tr("Error during CSV Export!\n\n"));
        QMessageBox::warning(this, QCoreApplication::applicationName(),
                                   msg,
                                   QMessageBox::Close);
    } else {
        msg.prepend(tr("CSV Exported\n\n"));
        QMessageBox::information(this, QCoreApplication::applicationName(),
                                   msg,
                                   QMessageBox::Close);
    }
}


void MainWindow::msgboxSumSize(int entries, int usingpmem, int bytes, int bits)
{
    QMessageBox::information(this, QCoreApplication::applicationName(),
                                   tr("%1 marked entries. %2 using pmem space.\n\n"
                                      "Used bytes: %3\n"
                                      "Used bits: %4\n"
                                     ).arg(entries).arg(usingpmem).arg(bytes).arg(bits),
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
        //qDebug() << " Recent file " << i << " : " << recentFilesList.at(i);
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
    //qDebug() << "GPCSelectedDir: " << gpcSelectedDir;
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
    pmemWindow->closeIt(); // <- required to close both windows when pmemwindow is created without parent
    QMainWindow::closeEvent(event);
}

void MainWindow::tableViewCreateCtxMenu()
{
    tvCtxFieldCopy = new QAction(tr("Copy Field Text"), this);
    connect(tvCtxFieldCopy, &QAction::triggered, this, &MainWindow::tableViewFieldCopy);
    tvCtxSelCopy = new QAction(tr("Copy Values (selected)"), this);
    connect(tvCtxSelCopy, &QAction::triggered, this, &MainWindow::tableViewSelectedCopy);
    tvCtxChkCopy = new QAction(tr("Copy Values (checked)"), this);
    connect(tvCtxChkCopy, &QAction::triggered, this, &MainWindow::tableViewCheckedCopy);
    tvCtxPaste = new QAction(tr("Paste Values"), this);
    connect(tvCtxPaste, &QAction::triggered, this, &MainWindow::tableViewPaste);
    tvCtxSelDelete = new QAction(tr("Delete Values (selected)"), this);
    connect(tvCtxSelDelete, &QAction::triggered, this, &MainWindow::tableViewSelectedDelete);
    tvCtxChkDelete = new QAction(tr("Delete Values (checked)"), this);
    connect(tvCtxChkDelete, &QAction::triggered, this, &MainWindow::tableViewCheckedDelete);
    tvCtxAllDelete = new QAction(tr("Delete Values (all)"), this);
    connect(tvCtxAllDelete, &QAction::triggered, this, &MainWindow::tableViewAllDelete);
    //--------------------------------------------------------------------
    tvCtxSelCheck = new QAction(tr("Check (selected)"), this);
    connect(tvCtxSelCheck, &QAction::triggered, this, &MainWindow::tableViewSelectedCheck);
    tvCtxSelUncheck = new QAction(tr("Uncheck (selected)"), this);
    connect(tvCtxSelUncheck, &QAction::triggered, this, &MainWindow::tableViewSelectedUncheck);
    tvCtxAllCheck = new QAction(tr("Check (all)"), this);
    connect(tvCtxAllCheck, &QAction::triggered, this, &MainWindow::tableViewAllCheck);
    tvCtxAllUncheck = new QAction(tr("Uncheck (all)"), this);
    connect(tvCtxAllUncheck, &QAction::triggered, this, &MainWindow::tableViewAllUncheck);
    //--------------------------------------------------------------------
    tvCtxSelSumSize = new QAction(tr("Sum Size (selected)"), this);
    connect(tvCtxSelSumSize, &QAction::triggered, this, &MainWindow::tableViewSelectedSumSize);
    tvCtxChkSumSize = new QAction(tr("Sum Size (checked)"), this);
    connect(tvCtxChkSumSize, &QAction::triggered, this, &MainWindow::tableViewCheckedSumSize);
    //--------------------------------------------------------------------
    tvCtxExportCSV = new QAction(tr("Export as CSV"), this);
    connect(tvCtxExportCSV, &QAction::triggered, this, &MainWindow::csvExport);
    //tvCtxExportCSV->setEnabled(false);

    tvCtxMenu = new QMenu;
    tvCtxMenu->addAction(tvCtxFieldCopy);
    tvCtxMenu->addAction(tvCtxSelCopy);
    tvCtxMenu->addAction(tvCtxChkCopy);
    tvCtxMenu->addAction(tvCtxPaste);
    tvCtxMenu->addAction(tvCtxSelDelete);
    tvCtxMenu->addAction(tvCtxChkDelete);
    tvCtxMenu->addAction(tvCtxAllDelete);
    tvCtxMenu->addSeparator();
    tvCtxMenu->addAction(tvCtxSelCheck);
    tvCtxMenu->addAction(tvCtxSelUncheck);
    tvCtxMenu->addAction(tvCtxAllCheck);
    tvCtxMenu->addAction(tvCtxAllUncheck);
    tvCtxMenu->addSeparator();
    tvCtxMenu->addAction(tvCtxSelSumSize);
    tvCtxMenu->addAction(tvCtxChkSumSize);
    tvCtxMenu->addSeparator();
    tvCtxMenu->addAction(tvCtxExportCSV);
}

void MainWindow::tableViewFieldCopy()
{
    tvCtxCopyIdx->clear();
    tvCtxCopyData->clear();
    tvCtxCopyDataHex->clear();

    QPoint pos = ui->tableView->viewport()->mapFromGlobal(tvCtxCursorLocation);
    QModelIndex mindex = icProxy->mapToSource(ui->tableView->indexAt(pos));
    //qDebug() << " Cursor Location: " << mindex.row() << " " << mindex.column();
    QString fieldText = icModel->index( mindex.row(), mindex.column() ).data( Qt::DisplayRole ).toString();

    tvCtxCopyIdx->append(QString::number(mindex.row()));
    tvCtxCopyData->append(fieldText);

    QString clipText = "Row|Col|Text\n";
    clipText.append(QString::number(mindex.row())).append("|");
    clipText.append(QString::number(mindex.column())).append("|");
    clipText.append(fieldText).append("\n");

    QClipboard *p_Clipboard = QApplication::clipboard();
    p_Clipboard->setText(clipText);
}

void MainWindow::tableViewSelectedCopy()
{
    tvCtxCopyIdx->clear();
    tvCtxCopyData->clear();
    tvCtxCopyDataHex->clear();

    QString clipText = "Row|Val|Hex\n";
    int idx=0;

    foreach(const QModelIndex &index,
            ui->tableView->selectionModel()->selectedRows())
    {
        IC ic = icModel->icData->data[icProxy->mapToSource(index).row()];
        tvCtxCopyIdx->append(QString::number(index.row()));
        if (ic.getNewVal().length()>0)
        {
            tvCtxCopyData->append(ic.getNewVal());
            tvCtxCopyDataHex->append(ic.getNewValHex());
        }
        else {
            tvCtxCopyData->append(ic.getDefaultVal());
            tvCtxCopyDataHex->append(ic.getDefaultValHex());
        }
        clipText.append(tvCtxCopyIdx->at(idx)).append("|");
        clipText.append(tvCtxCopyData->at(idx)).append("|");
        clipText.append(tvCtxCopyDataHex->at(idx)).append("\n");
        idx++;
    }

    QClipboard *p_Clipboard = QApplication::clipboard();
    p_Clipboard->setText(clipText);
}

void MainWindow::tableViewCheckedCopy()
{
    tvCtxCopyIdx->clear();
    tvCtxCopyData->clear();
    tvCtxCopyDataHex->clear();

    QString clipText = "Row|Val|Hex\n";
    int idx=0;

    for(int index = 0; index < icModel->icData->data.size(); index++)
    {
        IC ic = icModel->icData->data[index];
        if (ic.isChecked())
        {
            tvCtxCopyIdx->append(QString::number(index));
            if (ic.getNewVal().length()>0)
            {
                tvCtxCopyData->append(ic.getNewVal());
                tvCtxCopyDataHex->append(ic.getNewValHex());
            }
            else {
                tvCtxCopyData->append(ic.getDefaultVal());
                tvCtxCopyDataHex->append(ic.getDefaultValHex());
            }
            clipText.append(tvCtxCopyIdx->at(idx)).append("|");
            clipText.append(tvCtxCopyData->at(idx)).append("|");
            clipText.append(tvCtxCopyDataHex->at(idx)).append("\n");
            idx++;
        }
    }
}

void MainWindow::tableViewPaste()
{
    const QModelIndex index = ui->tableView->selectionModel()->selectedRows().first();
    //qDebug() << "selection at: " << index.row();
    int idx_first=0;
    for (int idx=0; idx<tvCtxCopyIdx->size();idx++)
    {
        /*
        qDebug() << "idx: " << tvCtxCopyIdx->at(idx) <<
                    " , val: " << tvCtxCopyData->at(idx) <<
                    " , hex: " << tvCtxCopyDataHex->at(idx);
                    */

        if (idx == 0) idx_first=tvCtxCopyIdx->at(idx).toInt();
        QModelIndex indexT = icProxy->index(index.row()+(tvCtxCopyIdx->at(idx).toInt()-idx_first),0);
        //qDebug() << "indexT: " << indexT.row();
        IC *ic = &icModel->icData->data[icProxy->mapToSource(indexT).row()];
        //qDebug() << "Paste at: " << ic->getName();
        if (tvCtxCopyData->size() >= idx+1) ic->newVal=tvCtxCopyData->at(idx);
        if (tvCtxCopyDataHex->size() >= idx+1) ic->newValHex=tvCtxCopyDataHex->at(idx);
    }
}

void MainWindow::tableViewSelectedDelete()
{
    foreach(const QModelIndex &index,
            ui->tableView->selectionModel()->selectedRows())
    {
        icModel->icData->data[icProxy->mapToSource(index).row()].newVal="";
        icModel->icData->data[icProxy->mapToSource(index).row()].newValHex="";
    }
}

void MainWindow::tableViewCheckedDelete()
{
    for(int idx = 0; idx < icModel->icData->data.size(); idx++)
    {
        if (icModel->icData->data[idx].checked)
        {
            icModel->icData->data[idx].newVal="";
            icModel->icData->data[idx].newValHex="";
        }
    }
}

void MainWindow::tableViewAllDelete()
{
    for(int idx = 0; idx < icModel->icData->data.size(); idx++)
    {
        icModel->icData->data[idx].newVal="";
        icModel->icData->data[idx].newValHex="";
    }
}


void MainWindow::tableViewSelectedCheck()
{
    foreach(const QModelIndex &index,
            ui->tableView->selectionModel()->selectedRows())
    {
        icModel->icData->data[icProxy->mapToSource(index).row()].checked=true;
    }
}

void MainWindow::tableViewSelectedUncheck()
{
    foreach(const QModelIndex &index,
            ui->tableView->selectionModel()->selectedRows())
    {
        icModel->icData->data[icProxy->mapToSource(index).row()].checked=false;
    }
}

void MainWindow::tableViewAllCheck()
{
    for(int idx = 0; idx < icModel->icData->data.size(); idx++)
    {
        icModel->icData->data[idx].checked=true;
    }
}

void MainWindow::tableViewAllUncheck()
{
    for(int idx = 0; idx < icModel->icData->data.size(); idx++)
    {
        icModel->icData->data[idx].checked=false;
    }
}

void MainWindow::tableViewSelectedSumSize()
{
    int entries=0, usingpmem=0;
    int bytes=0, bits=0;
    foreach(const QModelIndex &index,
            ui->tableView->selectionModel()->selectedRows())
    {
        entries++;
        IC ic = icModel->icData->data[icProxy->mapToSource(index).row()];
        usingpmem+=ic.getSize(&bytes,&bits);
    }
    msgboxSumSize(entries,usingpmem,bytes,bits);
}

void MainWindow::tableViewCheckedSumSize()
{
    int entries=0, usingpmem=0;
    int bytes=0, bits=0;
    for(int index = 0; index < icModel->icData->data.size(); index++)
    {
        IC ic = icModel->icData->data[index];
        if (ic.isChecked()) {
            entries++;
            usingpmem+=ic.getSize(&bytes,&bits);
        }
    }
    msgboxSumSize(entries,usingpmem,bytes,bits);
}

void MainWindow::csvExport()
{
    QDir curPath = QDir::currentPath();
    if (gpcSelectedDir.length() > 0) curPath.setPath(gpcSelectedDir);
    QString csvFileName =  QFileDialog::getSaveFileName(
              this,
              "Save CSV Export",
                curPath.path(),
              "CSV (*.csv *.txt);;All files (*.*)");

    if(!csvFileName.isNull())
    {
        bool csvData, csvChart;
        csvData = icModel->icData->exportCSV(csvFileName);
        csvChart = pmemWindow->exportCSV(csvFileName);
        msgboxCSVExport(csvData,csvChart);
    }
}

void MainWindow::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    if (ui->tableView->selectionModel()==nullptr) return;
    tvCtxCursorLocation = QCursor::pos();
    tvCtxMenu->exec(QCursor::pos());
}

void MainWindow::on_Import_clicked()
{
    if (icModel->icData == nullptr) return;
    QString cfgString = ui->lineEdit_Import->text();
    icModel->importConfigString(cfgString);
    icProxy->invalidate();
    icProxy->colSortNow();
}

void MainWindow::on_Export_clicked()
{
    ui->lineEdit_Export->setText(icModel->exportConfigString());
}
