#ifndef ICD_H
#define ICD_H

#include <QAbstractTableModel>
#include "ic.h"

class ICD
{
public:
    ICD();

    void debug();
    void debug(QString sName);
    void debug(qint32 iLineNo);

    bool validateUniqueNames();
    bool searchForNames(int srcidx, QString name, bool mark=true, QString msg="name is not unique", qint8 severity=8);

    QList<IC> data;
    qint16 bitsUsed;

};

#endif // ICD_H
