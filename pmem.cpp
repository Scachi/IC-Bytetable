#include "pmem.h"

PMEM::PMEM()
{
    this->bits[0]=0;
    this->bits[1]=0;
    this->bits[2]=0;
    this->bits[3]=0;
    this->bits[4]=0;
    this->bits[5]=0;
    this->bits[6]=0;
    this->bits[7]=0;

    this->line = -1;
    this->byByte=false;
    this->bySize=false;
    this->byBit=false;
}

PMEM::~PMEM()
{

}
