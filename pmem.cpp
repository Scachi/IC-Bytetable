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
        this->validBit[i]=true;
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

void PMEM::setInvalid() {
    this->valid=false;
}

bool PMEM::validate() {
    if (this->isByte() && this->isBit()) this->valid=false;

    if (this->getByByte() && this->getByByteSize()) this->valid=false;
    if (this->getByByte() > 1) this->valid=false;
    if (this->getByByteSize() > 1) this->valid=false;

    for (uint8_t i=0;i<8;i++) {
        if (this->byBit[i] && this->byBitSize[i]) {
            this->valid=false;
            this->validBit[i]=false;
        }
        if (this->byBit[i]>1) {
            this->valid=false;
            this->validBit[i]=false;
        }
        if (this->byBitSize[i]>1) {
            this->valid=false;
            this->validBit[i]=false;
        }
    }

    return this->valid;
}

QStringList PMEM::ErrMsgs(int bitsize, int bitoffset) {
    QStringList errmsgs;

    if (bitsize%8 == 0) {
        if (this->isByte() && this->isBit()) errmsgs.append("used as byte and bit\n");
        if (this->getByByte() && this->getByByteSize()) errmsgs.append("byte used direct and by size (overlap)\n");
        if (this->getByByte() > 1) errmsgs.append("byte used multiple times (direct)\n");
        if (this->getByByteSize() > 1) errmsgs.append("byte used multiple times (overlap)\n");
    } else {
        if (this->isByte() && this->isBit()) errmsgs.append("used as bit and byte\n");
        if (this->byBit[bitoffset] && this->byBitSize[bitoffset]) errmsgs.append("bit used direct and by size\n");
        if (this->byBit[bitoffset]>1) errmsgs.append("bit used multiple times (direct)\n");
        if (this->byBitSize[bitoffset]>1) errmsgs.append("bit used multiple times (overlap)\n");
    }
    return errmsgs;
}

int PMEM::getBits() const
{
    int bits=0;
    for (uint8_t i=0;i<8;i++) if (this->bits[i]) bits+=this->bits[i];
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

bool PMEM::isBit() const
{
    for (uint8_t i=0;i<8;i++) {
        if (this->byBit[i]) return true;
        if (this->byBitSize[i]) return true;
    }
    return false;
}

bool PMEM::isByte() const
{
    if (this->byByte) return true;
    if (this->byByteSize) return true;
    return false;
}

bool PMEM::isValid() const
{
    bool valid = this->valid;
    for (uint8_t i=0;i<8;i++)
        if (!this->validBit[i]) valid=false;
    return valid;
}

bool PMEM::isValidBit(int offset) const
{
    return this->validBit[offset];
}


void PMEM::debug(int offset)
{
    qDebug() << "Pmem " << offset << ": " <<
        QString("byByte(%1)").arg(this->byByte) <<
        QString("byByteSize(%1)").arg(this->byByteSize) <<
        QString("bits(%1)").arg(this->getBits());
}
