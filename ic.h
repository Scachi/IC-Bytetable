#ifndef IC_H
#define IC_H
#include <QStringList>
#include <QDebug>

class IC
{
public:
    IC();
    ~IC();
    IC(QStringList slRawSection, QString sFileName, QString sLineNo, QString sName, QString sControl);
    IC(QString sFileName, QString sLineNo, QString sName, QString sControl);   
    void debug();

    QString getFileNameFirst() const;
    QString getFileName() const;
    QString getFileNameFull() const;
    QString getFileNameLineNo() const;
    QString getFileNameFullLineNo() const;
    QString getLineNoFirst() const;
    QString getLineNo() const;
    QString getLineNoCount() const;
    QString getName() const;
    QString getNameToolTip() const;

    QString getByteOffset() const;
    QString getByteOffsetHex() const;
    QString getBitSize() const;
    QString getBitOffset() const;

    QString getValid() const;
    QString getInfo() const;
    QString getInfoToolTip() const;
    QString getInfoCode() const;

    QString getDefaultVal() const;
    QString getDefaultValHex() const;
    QString getNewVal() const;
    QString getNewValHex() const;

    QString getControl() const;
    QString getItemCount(QString prefix="") const;
    QString getItemNamesToolTip() const;
    QString getMinVal() const;
    QString getMaxVal() const;
    QString getDecimals() const;
    QString getStep() const;

    QString getShortDesc(int limit=10) const;
    QString getShortDescPlain(int limit=10) const;
    QString getCollapsible() const;
    QString getCollapsibleToolTip() const;
    QString getGroup() const;
    QString getGroupCol() const;
    QString getColor() const;
    QString getColorToolTip() const;
    QString getBorder() const;
/*
    QString getVarType() const;
    QString getVarName() const;
    QString getComment() const;
*/
    bool containsBRTags(QString source) const;
    qint8 reValidate();
    qint8 validate();
    qint8 validateControl();
    qint8 validateControlCommon();
    qint8 validateControlDefault();
    qint8 validateControlItem(bool checkdefault=true);
    qint8 validateControlValue();
    qint8 validateByteOffset();
    qint8 validateBitSize();
    qint8 validateBitOffset();

    qint8 infoAdd(QString msg);
    qint8 warnAdd(QString msg);
    qint8 errAdd(QString msg);
    qint8 errAdd(QStringList msg);
    qint8 msgAdd(QString msg, qint8 severity=8);

    bool isChecked();
    bool isUsingPMEM();
    bool getSize(int *bytes, int *bits);
    int canMerge(IC ic);
    bool merge(IC ic);

public:
    QStringList rawSection; // complete raw data of the section

    QStringList fileName;   // filename this section was found in

    QStringList lineNo;     // line number the [section] tag was found in
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

    /*
    QString     varType;  // for future usage - creation of ICs: vartype=
    QString     varName;  // for future usage - creation of ICs: varname=
    QString     comment;  // for future usage - creation of ICs: comment=
    */

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
