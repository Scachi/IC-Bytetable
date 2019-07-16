#include <QApplication>
#include <QClipboard>
#include <QRegExp>
#include "gpcreader.h"

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
    QClipboard *clipboard = QGuiApplication::clipboard();
    GPCRawList.append(clipboard->text());
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
    //qDebug() << "Interactive Configuration: begins at line:" << GPCICBegin << " and ends at line:" << GPCICEnd;
    if (GPCICBegin!= -1 && GPCICEnd!= -1) return true;
    return false;
}

void GPCReader::parseICRawList() {
    if (!gpcRawHasIC()) return; //ToDo: loop through all files
    ICRawList = GPCRawList.mid(GPCICBegin,GPCICBegin+GPCICEnd);
    //ToDO: parse IC content here

}
