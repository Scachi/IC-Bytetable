#ifndef ICV_H
#define ICV_H

#include <QVector>
#include "ic.h"

class ICV
{
public:
    ICV();

    void debug();
    void debug(QString sName);
    void debug(qint32 iLineNo);

    QVector<IC> vector;

};

#endif // ICV_H
