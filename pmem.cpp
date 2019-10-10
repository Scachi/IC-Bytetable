#include <qdebug.h>

#include "pmem.h"

PMEM::PMEM()
{
    this->filename = "";
    this->line = -1;
    this->byByte=0;
    this->byByteSize=0;
    this->valid=true;

    for (uint8_t i=0;i<8;i++) {
        this->bits[i]=0;
        this->byBit[i]=0;
        this->byBitSize[i]=0;
    }
}

PMEM::~PMEM()
{

}

bool PMEM::setByteByOffset(QString filename, QString line)
{
    this->filename=filename;
    this->line=line;
    this->byByte+=1;
    for (uint8_t i=0;i<8;i++) this->bits[i]+=1;
    return true;
}

bool PMEM::setByteBySize(QString filename, QString line)
{
    this->filename=filename;
    this->line=line;
    this->byByteSize+=1;
    for (uint8_t i=0;i<8;i++) this->bits[i]+=1;
    return true;
}

bool PMEM::setBitByOffset(QString filename, QString line, int bitoffset)
{
    this->filename=filename;
    this->line=line;
    this->byBit[bitoffset]+=1;
    this->bits[bitoffset]+=1;
    return true;
}

bool PMEM::setBitBySize(QString filename, QString line, int bitoffset)
{
    this->filename=filename;
    this->line=line;
    this->byBitSize[bitoffset]+=1;
    this->bits[bitoffset]+=1;
    return true;
}

int PMEM::getBits() const
{
    int bits=0;
    for (uint8_t i=0;i<8;i++) if (this->bits[i]) bits++;
    return bits;
}

uint8_t PMEM::getByByte() const
{
    return this->byByte;
}

uint8_t PMEM::getByByteSize() const
{
    return this->byByteSize;
}

uint8_t PMEM::getByBit(int offset) const
{
    return this->byBit[offset];
}

uint8_t PMEM::getByBitSize(int offset) const
{
    return this->byBitSize[offset];
}

uint8_t PMEM::getBit(int offset) const
{
    return this->bits[offset];
}

bool PMEM::isByte() const
{
    if (this->byByte) return true;
    if (this->byByteSize) return true;
    return false;
}

bool PMEM::isValid() const
{
    return this->valid;
}

void PMEM::debug(int offset)
{
    qDebug() << "Pmem " << offset << ": " <<
        QString("byByte(%1)").arg(this->byByte) <<
        QString("byByteSize(%1)").arg(this->byByteSize) <<
        QString("bits(%1)").arg(this->getBits());
}
