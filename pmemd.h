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
    bool exportCSV(QString filename);

    int getFreeBytes();
    int getFreeBits();
    int getBytes();
    int getBits();

    int getFieldCount();

private:
    int fieldCount = 8; // count of important data variables used by listview and csvexport
};

#endif // PMEMD_H
