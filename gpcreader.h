#ifndef GPCREADER_H
#define GPCREADER_H

#include <QFileDialog>
#include <QStringList>
#include <QDebug>

class GPCReader
{
private:
    QString     gpcFilePath;
    QStringList includeList;
    QStringList includeListDone;

    QStringList icFileList;
    QStringList icRawList;
    QStringList icLineNo;
    QStringList icNameList;
    QStringList icShortdescList;
    QStringList icByteOffsetList;
    QStringList icByteOffsetHexList;
    QStringList icBitSizeList;
    QStringList icBitOffsetList;
    QStringList icDefaultValueList;
    QStringList icDefaultValueHexList;
    QStringList icNewValueList;
    QStringList icNewValueHexList;
    QStringList icQontrolTypeList;
    QStringList icItemList;
    QStringList icMinValList;
    QStringList icMaxList;
    QStringList icStepList;
    QStringList icDecimalsList;

public:
    GPCReader(QString s) {
        //icNameList = QList<QString>();
        gpcFilePath = s;
        if (s=="clipboard") readClipboard();
        else readFile();
        parseRawList();
    }


    QStringList getICRawList() {
            return icRawList;
    }

    QStringList getICNameList() {
            return icNameList;
    }


private:
    void readFile();
    void readClipboard();
    void parseRawList();

};

#endif // GPCREADER_H
