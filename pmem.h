#ifndef PMEM_H
#define PMEM_H

#include <QString>
#include "icd.h"

class PMEM
{
public:
    PMEM();
    ~PMEM();

private:
    uint8_t byByte;
    uint8_t byByteSize;
    bool    valid;
    uint8_t bits[8];
    uint8_t byBit[8];
    uint8_t byBitSize[8];
    bool    validBit[8];

public:
    bool    setByteByOffset();
    bool    setByteBySize();
    bool    setBitByOffset(int bitoffset);
    bool    setBitBySize(int bitoffset);
    void    setInvalid();

    bool    validate();
    QStringList ErrMsgs(int bitsize, int bitoffset);

    int     getBits() const;
    uint8_t getByByte() const;
    uint8_t getByByteSize() const;
    uint8_t getByBit(int offset) const;
    uint8_t getByBitSize(int offset) const;
    uint8_t getBit(int offset) const;

    bool    isBit() const; // used by bits
    bool    isByte() const; // used by bytes

    bool    isValid() const; // report false on byte or bits error
    bool    isValidBit(int offset) const; // single bit report

    QString getTableMark(int bitoffset=0) const;

    void    debug(int offset);
};

#endif // PMEM_H
