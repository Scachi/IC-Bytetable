#include <QFile>
#include <QDebug>

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
        this->byteSet(ic.byteOffset,ic.bitSize,ic.bitOffset);
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

bool PMEMD::byteSet (QString byteoffset, QString bitsize, QString bitoffset)
{
    // skip entries without byteoffset or bitsize and invalid offsets
    if (byteoffset.length() == 0 || bitsize.length() == 0) return false;
    if (byteoffset.toInt() < 0 || byteoffset.toInt() > 127 ) return false;
    if (bitoffset.toInt() < 0 || bitoffset.toInt() > 7 ) return false;
    // bytes
    if (bitsize.toInt()%8 == 0) {
        data[byteoffset.toInt()].setByteByOffset();
        for (int i=1;i<bitsize.toInt()/8;i++)
        {
            data[byteoffset.toInt()+i].setByteBySize();
        }
    // bits
    } else {
        data[byteoffset.toInt()].setBitByOffset(bitoffset.toInt());
        if (bitsize.toInt()>1) {
            //qDebug() << "byte: " << byteoffset.toInt();
            //for (int i=bitoffset.toInt()+1;i<bitsize.toInt();i++)
            for (int i=1;i<bitsize.toInt();i++)
            {
                //qDebug() << "Setting " << byteoffset.toInt() << " size: " << i%8;
                data[byteoffset.toInt()+((bitoffset.toInt()+i)/8)].setBitBySize((bitoffset.toInt()+i)%8);
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
    if (byteoffset.toInt() < 0 || byteoffset.toInt() > 127 ) return errmsg;
    if (bitoffset.toInt() < 0 || bitoffset.toInt() > 7 ) return errmsg;

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

int PMEMD::getFieldCount()
{
    return fieldCount;
}

bool PMEMD::exportCSV(QString filename)
{
    QString csvQuote="\"";
    QString csvDelimiter=",";
    QString fname = filename.replace("_data.txt",".txt").replace("_chart.txt",".txt").replace(".txt","_chart.txt");
    fname = fname.replace("_data.csv",".csv").replace("_chart.csv",".csv").replace(".csv","_chart.csv");
    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&file);
    // column titles
    out << csvQuote << "Byte" << csvQuote << csvDelimiter;
    for (int col = 0; col < fieldCount ; col++)
    {
        if (col > 0) out << csvDelimiter;
        out << csvQuote << col << csvQuote;
    }
    out << csvDelimiter;
    out << csvQuote << "Status" << csvQuote;
    out << "\n";
    // content
    for (int row=0; row<this->data.size(); row++)
    {
        if (row > 0 ) out << "\n";
        out << csvQuote << row << csvQuote << csvDelimiter;
        for (int col=0; col < this->fieldCount ; col++)
        {
            if (col > 0) out << csvDelimiter;
            PMEM pm = data[row];
            out << csvQuote << pm.getTableMark(col) << csvQuote;
        }
        out << csvDelimiter;
        out << csvQuote;
        if (this->data[row].isValid()) out << "OK";
        else out <<  "ERROR";
        out << csvQuote;
    }

    return true;
}
