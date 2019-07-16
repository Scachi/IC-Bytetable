#ifndef GPC_H
#define GPC_H

#include <QList>
#include <QDebug>
#include "gpcreader.h"

class GPC
{
private:
    QString     GPCSelectedDir;        // the path part of the file that was initially opened
    QString     GPCSelectedFilePath;   // the file that was initially opened

    QStringList GPCIncludeList;        // list of found include files
    QStringList GPCIncludeListDone;    // include files that are already parsed

    // lists containing all the keywords and their values found in the parsed files
    QStringList ICFileList;
    QStringList ICLineNo;
    QStringList ICCommentList;      // OWN:single line only, line has to be directly above the [Name] and has to start with //
    QStringList ICNameList;
    QStringList ICShortdescList;
    QStringList ICByteOffsetList;
    QStringList ICByteOffsetHexList;
    QStringList ICBitSizeList;
    QStringList ICBitOffsetList;
    QStringList ICDefaultValueList;
    QStringList ICDefaultValueHexList;
    QStringList ICNewValueList;
    QStringList ICNewValueHexList;
    QStringList ICQontrolTypeList;
    QStringList ICItemList;
    QStringList ICMinValList;
    QStringList ICMaxValList;
    QStringList ICStepList;
    QStringList ICDecimalsList;
    QStringList ICVariableTypeList; // for future usage - creation of ICs: vartype=
    QStringList ICVariableNameList; // for future usage - creation of ICs: varname=


public:
    GPC(QString sDir, QString sFilePath) {
        GPCSelectedDir=sDir;
        GPCSelectedFilePath=sFilePath;
        GPCReader r = GPCReader(sDir,sFilePath);
        //icNameList = r->getICNames();
        //qDebug() << "r = GPCReader(s) : " << s.toUtf8();
    }

};

#endif // GPC_H
