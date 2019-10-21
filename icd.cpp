#include <QtDebug>
#include <QFile>
#include "icd.h"
#include "ic.h"
#include "xtra.h"

ICD::ICD()
{
    bitsUsed = 0;
    fieldNames << "Filename" << "Line" << "Name";
    fieldNames << "Byte\nOffset" << "Byte\nOffhex" << "Bit\nSize" << "Bit\nOffset";
    fieldNames << "Status" << "Info";
    fieldNames << "Def\nVal" << "Def\nHex" << "New\nVal" << "New\nHex";
    fieldNames << "Control" << "Items" << "Min\nVal" << "Max\nVal" << "Decimals" << "Step";
    fieldNames << "ShortDesc" << "Collapsible" << "Group" << "GroupCol" << "Color" << "Border";
    fieldCount = fieldNames.size();
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

void ICD::clear()
{
    this->data.clear();
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

int ICD::getBitsizeFromByteoffset(int byteoffset)
{
    for (int row=0; row<this->data.count(); row++)
    {
        if (data[row].byteOffset.length() > 0 && data[row].byteOffset.toInt()==byteoffset)
            return data[row].bitSize.toInt();
    }
    return 8;
}

QString ICD::getValHexFromByteoffset(bool *ok,int byteoffset)
{
    for (int row=0; row<this->data.count(); row++)
    {
        if (data[row].byteOffset.length() > 0 && data[row].byteOffset.toInt()==byteoffset) {
            *ok=true;
            if (data[row].newValHex.length()>0) return data[row].newValHex;
            else return data[row].defaultValHex;
        }
    }
    *ok=false;
    return "00";
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

bool ICD::exportCSV(QString filename)
{
    QString csvQuote="\"";
    QString csvDelimiter=",";
    QString fname = filename.replace("_data.txt",".txt").replace("_chart.txt",".txt").replace(".txt","_data.txt");
    fname = fname.replace("_data.csv",".csv").replace("_chart.csv",".csv").replace(".csv","_data.csv");
    QFile file(fname);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&file);
    // column titles
    out << csvQuote << "Index" << csvQuote << csvDelimiter;
    for (int col=0; col < this->fieldCount ; col++)
    {
        if (col>0) out << csvDelimiter;
        QString title;
        if (col < this->fieldNames.size())
            title = this->fieldNames.at(col);
        else title = QString::number(col);
        out << csvQuote << title.replace("\n"," ") << csvQuote;
    }
    out << "\n";
    // content
    for (int row=0; row<this->data.count(); row++)
    {
        if (row>0) out << "\n";
        out << csvQuote << row << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getFileName() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getLineNo() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getName() << csvQuote << csvDelimiter;

        out << csvQuote << data[row].getByteOffset() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getByteOffsetHex() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getBitSize() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getBitOffset() << csvQuote << csvDelimiter;

        out << csvQuote << data[row].getValid() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getInfo().replace("\n"," ") << csvQuote << csvDelimiter;

        out << csvQuote << data[row].getDefaultVal() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getDefaultValHex() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getNewVal() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getNewValHex() << csvQuote << csvDelimiter;

        out << csvQuote << data[row].getControl() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getItemCount() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getMinVal() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getMaxVal() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getDecimals() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getStep() << csvQuote << csvDelimiter;

        out << csvQuote << data[row].getShortDescPlain(99).replace("\n"," ") << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getCollapsible() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getGroup() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getGroupCol() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getColor() << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getBorder() << csvQuote;
    }
    return true;
}

int ICD::getDataCount()
{
    return data.size();
}

int ICD::getFieldCount()
{
    return fieldCount;
}

QString ICD::getFieldName(int index) const
{
    return fieldNames.at(index);
}

bool ICD::importConfigString(QString cfgstring)
{
    //qDebug() << "Import";
    if (cfgstring.startsWith("GIVICFG:"))
        cfgstring = cfgstring.mid(8);
    //qDebug() << "cfgstring: " << cfgstring;

    //parse the string - all values are hex
    int hexIndex;
    int byteOffset = 0;
    QString hexValue;
    int bitSize;

    for (hexIndex = 0; hexIndex < cfgstring.length() - 1; hexIndex++)
    {
        hexValue = cfgstring.mid(hexIndex, 1);
        // check if current character is a '#' then read nn (the byteoffset)
        if (hexValue.compare("#") ==0 ) // byteoffset found
        {
            bool ok;
            hexIndex += 1;
            hexValue = cfgstring.mid(hexIndex, 2);
            byteOffset = hexValue.toInt(&ok,16); // update byteoffset value
            //qDebug() << "ok:" << ok << "HexIndex: " << hexIndex << "Jumping to Byteoffset: " << byteOffset;
            hexIndex += 1; // increase to next hex to read/check
        }
        else if (hexValue.compare(":") == 0 ) // normal value definition
        {
            hexIndex += 1;
            bitSize = getBitsizeFromByteoffset(byteOffset);
            if (bitSize < 8) bitSize = 8;
            hexValue = cfgstring.mid(hexIndex, bitSize / 4);
            //qDebug() << "ImportValues byteoffset:'" << byteOffset << "' | hexvalue'" << hexValue; //4debug
            setByteoffset2Hex(byteOffset,hexValue);
            //qDebug() << "byteOffset: " << byteOffset << " (size:"<< bitSize <<") | Parsing string pos " << hexIndex << " - hex: " << hexValue;
            hexIndex += (bitSize / 4) - 1;  // update hex index position
            byteOffset += (bitSize / 8);    // update byteoffet position
        }
        else if (hexValue.compare("|") == 0 ) // bitwise OR (set a bit)
        {
            bool ok;
            hexIndex += 1;
            bitSize = getBitsizeFromByteoffset(byteOffset);
            if (bitSize < 8) bitSize = 8;
            hexValue = cfgstring.mid(hexIndex, bitSize / 4);
            //qDebug() << "ImportValues byteoffset:'" << byteOffset << "' | hexvalue'" << hexValue; //4debug
            QString binOR = QString("%1").arg(hexValue.toULongLong(&ok, 16), 8, 2, QChar('0'));
            QString hexSrc = getValHexFromByteoffset(&ok, byteOffset);
            //QString binSrc = QString("%1").arg(hexSrc.toULongLong(&ok, 16), 8, 2, QChar('0'));
            QString binSrc = XTRA::xHex2Bin(hexSrc,QString::number(bitSize));
            qDebug() << "BINARY OR: ";
            qDebug() << binSrc << " <- src mask";
            qDebug() << binOR << " <- mod mask";
            //setByteoffset2Hex(byteOffset,hexValue);
            //qDebug() << "byteOffset: " << byteOffset << " (size:"<< bitSize <<") | Parsing string pos " << hexIndex << " - hex: " << hexValue;
            hexIndex += (bitSize / 4) - 1;  // update hex index position
            byteOffset += (bitSize / 8);    // update byteoffet position
        }
        else if (hexValue.compare("&") == 0 ) // bitwise AND (clear a bit)
        {
            bool ok;
            hexIndex += 1;
            bitSize = getBitsizeFromByteoffset(byteOffset); // get bitsize
            if (bitSize < 8) bitSize = 8;
            hexValue = cfgstring.mid(hexIndex, bitSize / 4); // read the hexvalue
            //qDebug() << "ImportValues byteoffset:'" << byteOffset << "' | hexvalue'" << hexValue; //4debug
            QString binAND = QString("%1").arg(hexValue.toULongLong(&ok, 16), 8, 2, QChar('0'));
            QString hexSrc = getValHexFromByteoffset(&ok, byteOffset);
            //QString binSrc = QString("%1").arg(hexSrc.toULongLong(&ok, 16), 8, 2, QChar('0'));
            QString binSrc = XTRA::xHex2Bin(hexSrc,QString::number(bitSize));
            qDebug() << "BINARY AND: ";
            qDebug() << binSrc << " <- src mask";
            qDebug() << binAND << " <- mod mask";
            //setByteoffset2Hex(byteOffset,hexValue);
            //qDebug() << "byteOffset: " << byteOffset << " (size:"<< bitSize <<") | Parsing string pos " << hexIndex << " - hex: " << hexValue;
            hexIndex += (bitSize / 4) - 1;  // update hex index position
            byteOffset += (bitSize / 8);    // update byteoffet position
        } else {
            //hexIndex += 1;
            bitSize = getBitsizeFromByteoffset(byteOffset); // get bitsize
            if (bitSize < 8) bitSize = 8;
            hexValue = cfgstring.mid(hexIndex, bitSize / 4); // read the hexvalue
            //qDebug() << "ImportValues byteoffset:'" << byteOffset << "' | hexvalue'" << hexValue; //4debug
            setByteoffset2Hex(byteOffset,hexValue); // import the hexvalue
            //qDebug() << "byteOffset: " << byteOffset << " (size:"<< bitSize <<") | Parsing string pos " << hexIndex << " - hex: " << hexValue;
            hexIndex += (bitSize / 4) - 1; // update hex index position
            byteOffset += (bitSize / 8); // update byteoffet position
        }
    }
    return true;
}

QString ICD::exportConfigString() const
{
    QString exportString="none";
    return exportString;
}
