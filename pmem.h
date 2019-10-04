#ifndef PMEM_H
#define PMEM_H

#include <QString>

class PMEM
{
public:
    PMEM();
    uint8_t pAdd (uint8_t byteoffset, uint8_t bitsize);
    uint8_t pAdd (QString byteoffset, QString bitsize);
    uint8_t pAdd (uint8_t byteoffset, uint8_t bitsize, uint8_t bitoffset);
    uint8_t pAdd (QString byteoffset, QString bitsize, QString bitoffset);

private:
    uint8_t pmem[128][10]; // [n] byte , [][0-7] bit, [][8] errors
};

#endif // PMEM_H
