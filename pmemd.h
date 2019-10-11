#ifndef PMEMD_H
#define PMEMD_H

#include <qlist.h>
#include "pmem.h"

class PMEMD
{
public:
    PMEMD();
    ~PMEMD();

    QList<PMEM> data;

    bool byteSet (QString filename, QString line, QString byteoffset, QString bitsize, QString bitoffset);

    int getFreeBytes();
    int getFreeBits();
};

#endif // PMEMD_H
