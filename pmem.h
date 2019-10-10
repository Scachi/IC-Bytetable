#ifndef PMEM_H
#define PMEM_H

#include <QString>

class PMEM
{
public:
    PMEM();
    ~PMEM();

private:
    QString filename;   // for pointing back to the main listview entry
    QString line;       // for pointing back to the main listview entry
    uint8_t byByte;
    uint8_t byByteSize;
    uint8_t bits[8];
    uint8_t byBit[8];
    uint8_t byBitSize[8];
    bool    valid;

public:
    bool    setByteByOffset(QString filename, QString line);
    bool    setByteBySize(QString filename, QString line);
    bool    setBitByOffset(QString filename, QString line, int bitoffset);
    bool    setBitBySize(QString filename, QString line, int bitoffset);

    int     getBits() const;
    uint8_t getByByte() const;
    uint8_t getByByteSize() const;
    uint8_t getByBit(int offset) const;
    uint8_t getByBitSize(int offset) const;
    uint8_t getBit(int offset) const;

    bool    isByte() const;
    bool    isValid() const;

    void    debug(int offset);
};

#endif // PMEM_H
