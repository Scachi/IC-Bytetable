#ifndef PMEMD_H
#define PMEMD_H

#include <qlist.h>

#include "icd.h"
#include "pmem.h"

class PMEMD
{
public:
    PMEMD();
    ~PMEMD();

    QList<PMEM> data;

    bool update(ICD *icData);
    void validate();
    bool isValid();
    bool byteSet (QString filename, QString line, QString byteoffset, QString bitsize, QString bitoffset);
    QStringList byteCheck (QString byteoffset, QString bitsize, QString bitoffset);

    int getFreeBytes();
    int getFreeBits();
    int getBytes();
    int getBits();
};

#endif // PMEMD_H
