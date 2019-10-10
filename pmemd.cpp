#include <qdebug.h>

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

bool PMEMD::byteSet (QString filename, QString line, QString byteoffset, QString bitsize, QString bitoffset) {
    // skip entries without byteoffset or bitsize
    if (byteoffset.length() == 0 || bitsize.length() == 0) return false;

    // bytes
    if (bitsize.toInt()%8 == 0) {
        data[byteoffset.toInt()].setByteByOffset(filename,line);
        for (int i=1;i<bitsize.toInt()/8;i++)
        {
            data[byteoffset.toInt()+i].setByteBySize(filename,line);
        }
    // bits
    } else {
        data[byteoffset.toInt()].setBitByOffset(filename,line,bitoffset.toInt());
        if (bitsize.toInt()>1) {
            for (int i=bitoffset.toInt()+1;i<bitsize.toInt();i++)
            {
                data[byteoffset.toInt()].setBitBySize(filename,line,i%8);
            }
        }
    }
    // data[byteoffset.toInt()].debug(byteoffset.toInt());
    return true;
}
