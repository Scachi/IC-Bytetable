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

    QList<IC> data;

};

#endif // ICD_H
