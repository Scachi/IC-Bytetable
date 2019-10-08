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

    qint32 line; // for easy scrolling to the matching entry in the main listview
    bool byteSet (qint32 line, uint8_t byteoffset, uint8_t bitsize);
    bool byteSet (qint32 line, QString byteoffset, QString bitsize);
    bool byteSet (qint32 line, uint8_t byteoffset, uint8_t bitsize, uint8_t bitoffset);
    bool byteSet (QString line, QString byteoffset, QString bitsize, QString bitoffset);
};

#endif // PMEMD_H
