#ifndef IC_H
#define IC_H
#include <QStringList>
#include <QDebug>

class IC
{
public:
    IC();
    IC(QStringList SLRawSection, QString SFileName, QString SLineNo, QString SName, QString SControl);
    IC(QString SFileName, QString SLineNo, QString SName, QString SControl);

    void ICDebug(QVector<IC> ICVec);
    void ICDebug(QVector<IC> ICVec, QString sLineNo);
    void ICOut(IC);
    void ICOut();

private:

public:
    QStringList RawSection; // complete raw data of the section

    QString     FileName;   // filename this section was found in

    QString     LineNo;       // line number the [section] tag was found in
    QString     Name;       // name of the [section]
    QString     Control;
    QStringList Item;
    QString     MinVal;
    QString     MaxVal;
    QString     Step;
    QString     Decimals;

    QString     Collapsible;
    QString     Group;
    QString     GroupCol;
    QString     Color;

    QString     VarType;  // for future usage - creation of ICs: vartype=
    QString     VarName;  // for future usage - creation of ICs: varname=
    QString     Comment;  // for future usage - creation of ICs: comment=

    QStringList Shortdesc;
    QString     ByteOffset;
    QString     ByteOffsetHex;
    QString     BitSize;
    QString     BitOffset;
    QString     DefaultVal;
    QString     DefaultValHex;
    QString     NewVal;
    QString     NewValHex;

    qint8       Valid; // result of validate
    QStringList Info;  // information notes
    QStringList Warn;  // warning notes
    QStringList Err;   // error notes
};

#endif // IC_H
