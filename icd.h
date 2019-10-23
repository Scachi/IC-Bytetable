#ifndef ICD_H
#define ICD_H

//#include <QAbstractTableModel>
#include <qlist.h>
#include "ic.h"

class ICD
{
public:
    ICD();
    ~ICD();

    QList<IC> data;
    qint16 bitsUsed;

    void debug();
    void debug(QString sName);
    void debug(qint32 iLineNo);

    void clear();
    bool add(IC ic);
    int getIndexOfName(QString name, int startindex=0);

    bool validateUniqueNames();
    bool isValid();

    // the hex value is for the whole byte, so we need to look all bits up to create the correct hex value
    void bits2Hex();
    QString bits2ByteHex(QString byteoffset, bool defonly=true);
    bool setByteoffset2Hex(int byteoffset, QString hexvalue, bool bitsonly=false, bool setdefval=true);
    int getBitsizeFromByteoffset(int byteoffset);
    QString getValHexFromByteoffset(bool *ok, int byteoffset);

    bool exportCSV(QString filename);
    int getDataCount();
    int getFieldCount();
    QString getFieldName(int index) const;
    bool importConfigString(QString cfgstring);
    QString exportConfigString() const;

private:
    int fieldCount; // count of important IC data variables used by listview and csvexport
    QStringList fieldNames; // titles for the entries
    bool searchForNames(int srcidx, QString name, bool mark=true, QString msg="name is not unique", qint8 severity=8);

};

#endif // ICD_H
