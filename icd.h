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
    bool validateUniqueNames();
    void bits2Hex();
    QString bits2ByteHex(QString byteoffset);
    bool setByteoffset2Hex(int byteoffset, QString hexvalue, bool bitsonly=false);
    int getBitsizeFromByteoffset(int byteoffset);
    QString getValHexFromByteoffset(bool *ok, int byteoffset);
    bool isValid();

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
