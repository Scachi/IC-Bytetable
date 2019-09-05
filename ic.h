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

    QString getFileName() const;
    QString getFileNameFull() const;
    QString getLineNo() const;
    QString getName() const;

    QString getByteOffset() const;
    QString getByteOffsetHex() const;
    QString getBitSize() const;
    QString getBitOffset() const;

    QString getValid() const;
    QString getNotes() const;

    QString getDefaultVal() const;
    QString getDefaultValHex() const;
    QString getNewVal() const;
    QString getNewValHex() const;

    QString getControl() const;
    QString getItem(QString prefix="") const;
    QString getItemNames() const;
    QString getMinVal() const;
    QString getMaxVal() const;
    QString getDecimals() const;
    QString getStep() const;

    QString getShortDesc(int limit=10) const;
    QString getShortDescPlain(int limit=10) const;
    QString getCollapsible() const;
    QString getGroup() const;
    QString getGroupCol() const;
    QString getColor() const;
    QString getBorder() const;

    QString getVarType() const;
    QString getVarName() const;
    QString getComment() const;

    bool containsBRTags(QString source) const;

public:
    QStringList rawSection; // complete raw data of the section

    QString     fileName;   // filename this section was found in

    QString     lineNo;     // line number the [section] tag was found in
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
    QString     border;

    QString     varType;  // for future usage - creation of ICs: vartype=
    QString     varName;  // for future usage - creation of ICs: varname=
    QString     comment;  // for future usage - creation of ICs: comment=

    QStringList shortDesc;
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

    bool        checked; // checkbox state
};

#endif // IC_H
