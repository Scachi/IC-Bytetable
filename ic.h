#ifndef IC_H
#define IC_H
#include <QStringList>
#include <QDebug>

class IC
{
public:
    IC();
    IC(QStringList slRawSection, QString sFileName, QString sLineNo, QString sName, QString sControl);
    IC(QString sFileName, QString sLineNo, QString sName, QString sControl);

    void debug();


public:
    QStringList rawSection; // complete raw data of the section

    QString     fileName;   // filename this section was found in

    QString     lineNo;       // line number the [section] tag was found in
    QString     name;       // name of the [section]
    QString     control;
    QStringList item;
    QString     minVal;
    QString     maxVal;
    QString     step;
    QString     decimals;

    QString     collapsible;
    QString     group;
    QString     groupCol;
    QString     color;

    QString     varType;  // for future usage - creation of ICs: vartype=
    QString     varName;  // for future usage - creation of ICs: varname=
    QString     comment;  // for future usage - creation of ICs: comment=

    QStringList shortdesc;
    QString     byteOffset;
    QString     byteOffsetHex;
    QString     bitSize;
    QString     bitOffset;
    QString     defaultVal;
    QString     defaultValHex;
    QString     newVal;
    QString     newValHex;

    qint8       valid; // result of validate
    QStringList info;  // information notes
    QStringList warn;  // warning notes
    QStringList err;   // error notes
};

#endif // IC_H
