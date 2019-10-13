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

bool PMEMD::update(ICD *icData)
{
    for(int idx = 0; idx < icData->data.size(); idx++)
    {
        IC ic = icData->data[idx];
        this->byteSet(ic.fileName,ic.lineNo,ic.byteOffset,ic.bitSize,ic.bitOffset);
    }
    this->validate();
    for(int idx = 0; idx < icData->data.size(); idx++)
    {
        IC ic = icData->data[idx];
        QStringList errmsg=this->byteCheck(ic.byteOffset,ic.bitSize,ic.bitOffset);
        if (errmsg.size()>0) icData->data[idx].errAdd(errmsg);
    }

    if (!this->isValid()) return false;
    return true;
}

bool PMEMD::byteSet (QString filename, QString line, QString byteoffset, QString bitsize, QString bitoffset)
{
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

QStringList PMEMD::byteCheck (QString byteoffset, QString bitsize, QString bitoffset)
{
    QStringList errmsg;
    // skip entries without byteoffset or bitsize
    if (byteoffset.length() == 0 || bitsize.length() == 0) return errmsg;

    // bytes
    if (bitsize.toInt()%8 == 0) {
        for (int i=0;i<bitsize.toInt()/8;i++)
        {
            errmsg.append(data[byteoffset.toInt()+i].ErrMsgs(bitsize.toInt(),bitoffset.toInt()));
        }
    // bits
    } else {
        //data[byteoffset.toInt()].setBitByOffset(filename,line,bitoffset.toInt());
        errmsg.append(data[byteoffset.toInt()].ErrMsgs(bitsize.toInt(),bitoffset.toInt()));
        if (bitsize.toInt()>1) {
            for (int i=bitoffset.toInt()+1;i<bitsize.toInt();i++)
            {
                errmsg.append(data[byteoffset.toInt()].ErrMsgs(bitsize.toInt(),i%8));
            }
        }
    }

    return errmsg;
}

void PMEMD::validate()
{
    for (int i=0;i<128;i++) data[i].validate();
}

bool PMEMD::isValid()
{
    bool valid=true;
    for (int i=0;i<128;i++) if (!data[i].isValid()) valid=false;
    return valid;
}


int PMEMD::getFreeBytes()
{
    int bytes=0;
    for (int i=0;i<128;i++) {
        if (!data[i].isByte() && !data[i].getBits()) bytes++;
    }
    return bytes;
}

int PMEMD::getFreeBits()
{
    int bits=0;
    int used=0;
    for (int i=0;i<128;i++) {
        used=0;
        for (int i2=0; i2<8; i2++)
            if (data[i].getByBit(i2) || data[i].getByBitSize(i2))
                used+=data[i].getBit(i2);
        if (used) bits+=8-used;
    }
    return bits;
}

int PMEMD::getBytes()
{
    int bytes=0;
    for (int i=0;i<128;i++) {
        if (data[i].isByte()) bytes+=data[i].getByByte()+data[i].getByByteSize();
    }
    return bytes;
}

int PMEMD::getBits()
{
    int bits=0;
    for (int i=0;i<128;i++) {
        for (int i2=0; i2<8; i2++)
            if (data[i].getByBit(i2) || data[i].getByBitSize(i2)) bits+=data[i].getBit(i2);
    }
    return bits;
}
