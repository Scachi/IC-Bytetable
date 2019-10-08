#include "pmemd.h"
#include "pmem.h"

PMEMD::PMEMD()
{
    for (int i=0;i<128;i++) {
        PMEM newPMEM;
        data.append(newPMEM);
    }
}

PMEMD::~PMEMD()
{

}

bool PMEMD::byteSet (qint32 line, uint8_t byteoffset, uint8_t bitsize) {
    return true;
}

bool PMEMD::byteSet (qint32 line, QString byteoffset, QString bitsize) {
    return true;
}

bool PMEMD::byteSet (qint32 line, uint8_t byteoffset, uint8_t bitsize, uint8_t bitoffset) {
    return true;
}

bool PMEMD::byteSet (QString line, QString byteoffset, QString bitsize, QString bitoffset) {
    for (uint8_t i=0;i<bitsize;i++)
    {
        if (i<8) data[byteoffset.toInt()].bits[i]+=1;
    }
    return true;
}

