#ifndef GPC_H
#define GPC_H

#include <QList>
#include <QDebug>
#include "gpcreader.h"

class GPC
{
private:
    QString     gpcFilePath;        // the file that was initially opened
    QStringList includeList;        // list of found include files
    QStringList includeListDone;    // include files that are already parsed

    // lists containing all the keywords and their values found in the parsed files
    QStringList icFileList;
    QStringList icLineNo;
    QStringList icCommentList;      // OWN:single line only, line has to be directly above the [Name] and has to start with //
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
    QStringList icMaxValList;
    QStringList icStepList;
    QStringList icDecimalsList;
    QStringList icVariableTypeList; // for future usage - creation of ICs: vartype=
    QStringList icVariableNameList; // for future usage - creation of ICs: varname=


public:
    GPC(QString s) {
        /*
        GPCReader *r = new GPCReader(s);
        //icNameList = r->getICNames();
        qDebug() << "*r = new GPCReader(s) : " << s.toUtf8();
        */
        gpcFilePath=s;

        GPCReader r = GPCReader(s);
        //icNameList = r->getICNames();
        qDebug() << "r = GPCReader(s) : " << s.toUtf8();
    }
};

#endif // GPC_H
