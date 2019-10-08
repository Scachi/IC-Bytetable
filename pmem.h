#ifndef PMEM_H
#define PMEM_H

#include <QString>

class PMEM
{
public:
    PMEM();
    ~PMEM();

public:
    uint8_t bits[8];
    qint32  line;
    bool    byByte; // usage by direct byte,
    bool    bySize; // usage by bitsize > 8
    bool    byBit;  // usage by bitsize < 8

};

#endif // PMEM_H
