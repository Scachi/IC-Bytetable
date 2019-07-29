#ifndef IC_H
#define IC_H
#include <QStringList>
#include <QDebug>

class IC
{
public:
    IC();

private:

    QStringList ICRawSection; // complete raw data of the section

    QString     ICFileName;   // filename this section was found in

    qint32      ICLine;       // line number the [section] tag was found in
    QString     ICName;       // name of the [section]
    QString     ICControl;
    QStringList ICItem;
    QString     ICMinVal;
    QString     ICMaxVal;
    QString     ICStep;
    QString     ICDecimals;

    QString     ICCollapsible;
    QString     ICGroup;
    QString     ICGroupCol;
    QString     ICColor;

    QString     ICVarType;  // for future usage - creation of ICs: vartype=
    QString     ICVarName;  // for future usage - creation of ICs: varname=
    QString     ICComment;  // for future usage - creation of ICs: comment=

    QStringList ICShortdesc;
    QString     ICByteOffset;
    QString     ICByteOffsetHex;
    QString     ICBitSize;
    QString     ICBitOffset;
    QString     ICDefaultVal;
    QString     ICDefaultValHex;
    QString     ICNewVal;
    QString     ICNewValHex;

    qint8       Valid; // result of validate
    QStringList Info;  // information notes
    QStringList Warn;  // warning notes
    QStringList Err;   // error notes
};

#endif // IC_H
