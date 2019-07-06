#ifndef GPC_H
#define GPC_H

#include <QList>
#include <QDebug>
#include "gpcreader.h"

class GPC
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
    GPC(QString s) {
        /*
        GPCReader *r = new GPCReader(s);
        //icNameList = r->getICNames();
        qDebug() << "*r = new GPCReader(s) : " << s.toUtf8();
        */
        GPCReader r = GPCReader(s);
        //icNameList = r->getICNames();
        qDebug() << "r = GPCReader(s) : " << s.toUtf8();
    }
};

#endif // GPC_H
