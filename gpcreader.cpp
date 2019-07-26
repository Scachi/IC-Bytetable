#include <QApplication>
#include <QClipboard>
#include <QRegExp>
#include <QMimeData>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include "gpcreader.h"

// tries to open the file : 1. as relative path, 2. as the string is
QString GPCReader::locateFile(QString path) {
    QString file2open=GPCCurrentFilePath;
    // try the selected files directory first (for relative path)
    if (path.length()>0) file2open.prepend(GPCSelectedDir);
    QFile inputFile(file2open);
    if (inputFile.exists()) return file2open;
    // as fallback try the filename as it was found in the #include string (for absolute path)
    file2open=GPCCurrentFilePath;
    return file2open;
}

void GPCReader::addPath(QFile file) {
    if (file.exists()) {
        GPCPathList.append(QFileInfo(file).absolutePath().append("/"));
        GPCPathList.removeDuplicates();
    }
}

void GPCReader::addPath(QString path) {
    QFile inputFile(path);
    if (inputFile.exists()) {
        GPCPathList.append(QFileInfo(GPCCurrentFilePath).absolutePath().append("/"));
        GPCPathList.removeDuplicates();
    }
}

void GPCReader::readFile(QString path) {
    QString file2open=locateFile(path);
    QFile inputFile(file2open);
    if (inputFile.exists()) qDebug() << "Going to read: " << file2open;
    else qDebug() << "Failed to find: " << file2open;
    inputFile.open(QIODevice::ReadOnly);    
    QTextStream in(&inputFile);
    QString line;
    // loop through all lines
    GPCRawList.clear();
    while (!in.atEnd()) {
        GPCRawList.append(in.readLine());
    }

    inputFile.close();
}


void GPCReader::readClipboard() {
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->hasText()) {
        GPCRawList.clear();
        QString sClip=clipboard->text();
        GPCRawList=sClip.split("\n");
        /*
        for (int i = 0; i < GPCRawList.size(); ++i) {
            qDebug() << "GPCRawList: " << GPCRawList[i];
        }
        */
    }

}

void GPCReader::parse() {
    if (GPCCurrentFilePath=="") {
        GPCCurrentFilePath=GPCSelectedFilePath;
        if (GPCSelectedFilePath=="clipboard") {
            readClipboard();
        } else {
            readFile();
        }
    }

    parseGPCRawList();
    parseICRawList();

    while (IncludeList.size()) {
        for (int i = 0; i < IncludeList.size(); ++i) {
            if (!IncludeListDone.contains(IncludeList[i])) {
                IncludeListDone.append(IncludeList[i]);
                GPCCurrentFilePath=IncludeList[i];
                IncludeList.removeAt(i);
                readFile(GPCSelectedDir);
                parseGPCRawList();
                parseICRawList();
            }
        }
    }
}


void GPCReader::parseGPCRawList() {
    findHeaderFiles(GPCRawList);
    //qDebug() << "IncludeList size: " << IncludeList.size();
    /*
    for (int i = 0; i < IncludeList.size(); ++i) {
        qDebug() << "Includes: " << IncludeList[i];
    }
    */
}

// search for lines: #include "files"
void GPCReader::findHeaderFiles(QStringList source) {
    QRegExp rx;
    rx.setPattern("\\s*#include\\s*\"(.*)\"");
    QStringList TMPList;
    TMPList=source.filter(rx);
    for (int i = 0; i < TMPList.size(); ++i) {
        if (rx.indexIn(TMPList.at(i)) != -1) {
            if (!IncludeList.contains(rx.cap(1)) && !IncludeListDone.contains(rx.cap(1))) IncludeList.append(rx.cap(1));
        }
    }
}

bool GPCReader::gpcRawHasIC() {
    // check if file contains <cfgdesc></cfgdesc> section
    QRegExp rx;
    rx.setPattern("<cfgdesc>");
    GPCICBegin=GPCRawList.indexOf(rx);
    rx.setPattern("</cfgdesc>");
    GPCICEnd=GPCRawList.indexOf(rx);
    qDebug() << "Interactive Configuration: begins at line:" << GPCICBegin << " and ends at line:" << GPCICEnd;
    if (GPCICBegin!= -1 && GPCICEnd!= -1) return true;
    return false;
}

void GPCReader::parseICRawList() {
    qDebug() << "parseICRawList";
    if (!gpcRawHasIC()) return;
    qDebug() << "gpcRawHasIC";
    GPCICFound=true;
    ICRawList = GPCRawList.mid(GPCICBegin,GPCICBegin+GPCICEnd);
    //ToDO: parse ICRawList IC content here
    findICNameLines();
}


void GPCReader::findICNameLines() {
    qDebug() << "FindICNames";

    QRegularExpression re;
    QRegularExpressionMatch match;
    qint32 line;
    re.setPattern("^\\s*(\\[.*\\])");
    line=0;
    while ((line=ICRawList.indexOf(re,line)) > -1) {
        match=re.match(ICRawList[line]);
        //qDebug() << "Found Name " << match.captured(0) << " in line: " << line;
        ICLineNo.append(QString::number(line));
        match=re.match(ICRawList[line]);
        ICNameList.append(match.captured(0));
        line++;
    }
}
