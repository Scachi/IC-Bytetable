#include <QApplication>
#include <QClipboard>
#include "gpcreader.h"

void GPCReader::readFile() {
    QFile inputFile(gpcFilePath);
    qDebug() << "gpcreader::readFile " << gpcFilePath.toUtf8();
    qDebug() << "gpc file exists: " << inputFile.exists() \
             << ", size: " << inputFile.size();

    inputFile.open(QIODevice::ReadOnly);    
    QTextStream in(&inputFile);
    QString line;
    // loop through all lines
    while (!in.atEnd()) {
        icRawList.append(in.readLine());
        //qDebug() << "line: " << line.toUtf8();
    }

    inputFile.close();
    qDebug() << "gpcreader::readfile closed";
}

void GPCReader::readClipboard() {
    QClipboard *clipboard = QGuiApplication::clipboard();
    icRawList.append(clipboard->text());
}


void GPCReader::parseRawList() {
    // walking a QStringList line by line

    // loop through all lines, check each line for data and push to each QList<QString> ic'listname'
    for (int i = 0; i < icRawList.size(); ++i) {
             qDebug() << icRawList.at(i).toLocal8Bit().constData();
    }

}

