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

    void debug();
    void debug(QString sName);
    void debug(qint32 iLineNo);

    bool validateUniqueNames();
    bool searchForNames(int srcidx, QString name, bool mark=true, QString msg="name is not unique", qint8 severity=8);
    bool isValid();

    QList<IC> data;
    qint16 bitsUsed;

};

#endif // ICD_H
