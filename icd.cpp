#include <QtDebug>
#include "icd.h"
#include "ic.h"
#include "xtra.h"

ICD::ICD()
{
    bitsUsed=0;
}

ICD::~ICD()
{

}

// qDebug() complete IC
void ICD::debug()
{
    for(int idx = 0; idx < data.size(); idx++)
    {
        data[idx].debug();
    }
}

// qDebug() specific IC entry by Name
void ICD::debug(QString sName)
{
    for(int idx = 0; idx < data.size(); idx++)
    {
        if( data[idx].name.compare(sName)==0 )
            data[idx].debug();
    }
}

// qDebug() specific IC entry by lineNo
void ICD::debug(qint32 iLineNo)
{
    for(int idx = 0; idx < data.size(); idx++)
    {
        if(data[idx].lineNo.toInt() == iLineNo)
            data[idx].debug();
    }
}

// validate the names of all controls to be unique
bool ICD::validateUniqueNames()
{
    QStringList namesProcessed;
    bool unique=true;
    for(int idx = 0; idx < data.size(); idx++)
    {
        if (namesProcessed.contains(data[idx].name)) continue;
        namesProcessed.append(data[idx].name);
        if (!searchForNames(idx,data[idx].name,true,QString("name %1 not unique").arg(data[idx].name)))
        {
            unique=false;
        }
    }
    return unique;
}

bool ICD::searchForNames(int srcid, QString name, bool mark, QString msg, qint8 severity)
{
    bool unique=true;
    for(int idx = 0; idx < data.size(); idx++)
    {
        if (idx==srcid) continue;
        if (data[idx].name.compare(name)==0)
        {
            unique=false;
            if (mark) data[idx].msgAdd(msg,severity);
        }
    }
    if (!unique && mark) data[srcid].msgAdd(msg,severity);
    return unique;
}

// the hex value is for the whole byte, so we need to look all bits up to create the correct hex value
void ICD::bits2Hex()
{
    QStringList bytesProcessed;
    QString byteHex;
    for(int idx = 0; idx < data.size(); idx++)
    {
        if (data[idx].byteOffset.length()==0) continue;
        if (data[idx].bitSize.toInt()>=8) continue;
        if (bytesProcessed.contains(data[idx].byteOffset)) continue;
        bytesProcessed.append(data[idx].byteOffset);
        byteHex = bits2ByteHex(data[idx].byteOffset);
        setByteoffset2Hex(data[idx].byteOffset.toInt(),byteHex,true);
    }
}

QString ICD::bits2ByteHex(QString byteoffset)
{
    QStringList bits={"0","0","0","0","0","0","0","0"};
    for(int idx = 0; idx < data.size(); idx++)
    {
        if (data[idx].byteOffset.length()==0) continue;
        if (data[idx].bitSize.toInt()>=8) continue;
        if (data[idx].byteOffset.compare(byteoffset)==0)
        {
            //ToDo: convert bitsize bits to bitmask and use it to fill bits[]
            QString sHex = XTRA::x2Hex(data[idx].defaultVal,data[idx].bitSize);
            QString sBin = XTRA::xHex2Bin(sHex,"8");
            //qDebug() << "sHex:" << sHex << " , sBin:" << sBin;
            int inbit=7;
            for (int outbit=data[idx].bitOffset.toInt();
                 outbit<data[idx].bitOffset.toInt()+data[idx].bitSize.toInt();
                 outbit++)
            {
                if (outbit>7) break;
                bits[7-outbit]=sBin.mid(inbit,1);
                inbit--;
            }
        }
    }
    QString bitmask = bits.join("");
    QString byte = XTRA::xBin2Byte(bitmask);
    QString byteHex = XTRA::x2Hex(byte,"8");
    //qDebug() << "byte " << byteoffset << "  :" << bitmask << " : " << byte << " : " << byteHex;
    return byteHex;
}

bool ICD::setByteoffset2Hex(int byteoffset, QString hexvalue, bool bitsonly)
{
    bool found=false;
    for(int idx = 0; idx < data.size(); idx++)
    {
        if (data[idx].byteOffset.length() > 0  && data[idx].byteOffset.toInt()==byteoffset)
        {
            if (bitsonly &&  data[idx].bitSize.toInt()>=8) continue;
            //qDebug() << " Setting " << icData[row].byteOffset << " to " << hexvalue;
            data[idx].defaultValHex = hexvalue;
            found = true;
        }
    }
    return found;
}


bool ICD::isValid()
{
    bool valid=true;
    for(int idx = 0; idx < data.size(); idx++)
    {
        if (data[idx].valid>2) valid=false;
    }
    return valid;
}
