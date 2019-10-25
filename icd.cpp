#include <QtDebug>
#include <QFile>
#include <QTranslator>
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
        if(data[idx].lineNo[0].toInt() == iLineNo)
            data[idx].debug();
    }
}

void ICD::clear()
{
    this->data.clear();
}

bool ICD::add(IC ic)
{
    int matchIdx = -1;
    if ((matchIdx = getIndexOfName(ic.name)) == -1) data.append(ic); // name not found
    else {  // name found, check if the new one defines unique keywords or if it collides
        /*
        qDebug() << "canmerge: " << data[matchIdx].canMerge(ic);
        qDebug() << "shortdesc1:" << ic.shortDesc;
        qDebug() << "shortdesc2:" << data[matchIdx].shortDesc;
        */
        if (data[matchIdx].canMerge(ic) == 0) {
            data[matchIdx].merge(ic);
            data[matchIdx].reValidate();
        } else {
            // add error notes and add it as new ? or let validate do it
            data.append(ic);
        }
    }
    return true;
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

int ICD::getIndexOfName(QString name, int startindex)
{
    for(int idx = startindex; idx < data.size(); idx++)
    {
        if (data[idx].name.compare(name)==0) return idx;
    }
    return -1;
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

QString ICD::bits2ByteHex(QString byteoffset, bool defonly)
{
    QStringList bits={"0","0","0","0","0","0","0","0"};
    for(int idx = 0; idx < data.size(); idx++)
    {
        if (data[idx].byteOffset.length()==0) continue;
        if (data[idx].bitSize.toInt()>=8) continue;
        if (data[idx].byteOffset.compare(byteoffset)==0)
        {
            // default: defaultVal
            QString sHex = XTRA::x2Hex(data[idx].defaultVal,data[idx].bitSize);
            // newVal only when allowed and found
            if (!defonly && data[idx].newVal.length() >0 )
                sHex = XTRA::x2Hex(data[idx].newVal,data[idx].bitSize);
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

bool ICD::setByteoffset2Hex(int byteoffset, QString hexvalue, bool bitsonly, bool setdefval)
{
    bool found=false;
    for(int idx = 0; idx < data.size(); idx++)
    {
        if (data[idx].byteOffset.length() > 0  && data[idx].byteOffset.toInt()==byteoffset)
        {
            if (bitsonly &&  data[idx].bitSize.toInt()>=8) continue;
            //qDebug() << " Setting " << data[idx].byteOffset << " to " << hexvalue;
            if (setdefval) data[idx].defaultValHex = hexvalue;
            else
            {
                data[idx].newValHex = hexvalue;
                data[idx].newHexToVal();
            }
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
        out << csvQuote << data[row].getFileName().replace("\n"," ") << csvQuote << csvDelimiter;
        out << csvQuote << data[row].getLineNo().replace("\n"," ") << csvQuote << csvDelimiter;
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
        if (hexValue.compare("#") == 0 ) // byteoffset found
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
            setByteoffset2Hex(byteOffset,hexValue,false,false);
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
            QString binRes = XTRA::xBinOR(binSrc,binOR);
            hexValue = XTRA::xBin2Hex(binRes);
            /*
            qDebug() << "BINARY OR: ";
            qDebug() << binSrc << " <- src mask";
            qDebug() << binOR << " <- mod mask";
            qDebug() << binRes << " <- result";
            qDebug() << hexValue << " <- hex result";
            */
            setByteoffset2Hex(byteOffset,hexValue,false,false);
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
            QString binRes = XTRA::xBinAND(binSrc,binAND);
            hexValue = XTRA::xBin2Hex(binRes);
            /*
            qDebug() << "BINARY AND: ";
            qDebug() << binSrc << " <- src mask";
            qDebug() << binAND << " <- mod mask";
            qDebug() << binRes << " <- result";
            qDebug() << hexValue << " <- hex result";
            */
            setByteoffset2Hex(byteOffset,hexValue,false,false);
            //qDebug() << "byteOffset: " << byteOffset << " (size:"<< bitSize <<") | Parsing string pos " << hexIndex << " - hex: " << hexValue;
            hexIndex += (bitSize / 4) - 1;  // update hex index position
            byteOffset += (bitSize / 8);    // update byteoffet position
        } else {
            //hexIndex += 1;
            bitSize = getBitsizeFromByteoffset(byteOffset); // get bitsize
            if (bitSize < 8) bitSize = 8;
            hexValue = cfgstring.mid(hexIndex, bitSize / 4); // read the hexvalue
            //qDebug() << "ImportValues byteoffset:'" << byteOffset << "' | hexvalue'" << hexValue; //4debug
            setByteoffset2Hex(byteOffset,hexValue,false,false); // import the hexvalue
            //qDebug() << "byteOffset: " << byteOffset << " (size:"<< bitSize <<") | Parsing string pos " << hexIndex << " - hex: " << hexValue;
            hexIndex += (bitSize / 4) - 1; // update hex index position
            byteOffset += (bitSize / 8); // update byteoffet position
        }
    }
    return true;
}

QString ICD::createConfigString(bool all) const
{
    QString exportStrA[128];
    int iChecked=0;
    int iNewValHex=0;

    // count checked items and items with new hex value
    for(int idx = 0; idx < data.size(); idx++)
    {
        IC ic = data[idx];
        if (ic.getByteOffset().length() == 0) continue;
        if (ic.isChecked()) iChecked++;
        if (ic.getNewValHex().length()>0) iNewValHex++;
    }

    if (iChecked == 0 && iNewValHex == 0) return ("Error: Nothing selected and no New Values set.");

    // fill exportStrA array
    for(int idx = 0; idx < data.size(); idx++)
    {
        IC ic = data[idx];
        if (ic.getByteOffset().length() == 0) continue;
        if (iChecked == 0) // no checked items, export all items with new hex value
        {
            if (ic.getNewValHex().length()>0) exportStrA[ic.getByteOffset().toInt()] = ic.getNewValHex();
        }
        else if (ic.isChecked()) // only export checked items
        {
            exportStrA[ic.getByteOffset().toInt()] = ic.getDefaultValHex();
            if (ic.getNewValHex().length()>0) exportStrA[ic.getByteOffset().toInt()] = ic.getNewValHex();
        }
    }

    // created GVICFG string in correct syntax
    QString exportString = "GVICFG:";
    bool first = true;
    for(int idx = 0; idx < 128; idx++)
    {
        if (!all && exportStrA[idx].length() == 0) continue;
        if (all && exportStrA[idx].length() == 0) exportStrA[idx]="00";

        if (first and idx > 0) exportString += "#" + XTRA::x2Hex(idx,8) + ":";
        first = false;
        exportString += exportStrA[idx];
        if (!all && exportStrA[idx+1].length() == 0) first = true;
    }
    return exportString;

    /*
        For $iIndex = 0 To UBound($exportstring) - 1
            If StringLen($exportstring[$iIndex]) > 0 Then
                ; first or sperate byteoffset entry needs a new #offset string
                If $first = 1 And $iIndex > 0 Then
                    $export &= "#" & Hex($iIndex, 2) & ":"
                    $first = 0
                Else
                    $first = 0
                EndIf
                $export &= $exportstring[$iIndex]

                ; check if the next value is directly after this one or we need a new #offset string
                Local $size = StringLen($exportstring[$iIndex]) * 0.5
                ;ConsoleWrite("size + index : " & $size & " + " & $iIndex & " = " & $iIndex + $size & @CRLF)
                If $iIndex + $size > 128 Or $iIndex + $size < 0 Then ContinueLoop
                if StringLen($exportstring[$iIndex + $size]) = 0 then $first = 1

            EndIf
        Next
        ; output the string
        ;ConsoleWrite("Exportstring:" & $export & @CRLF) ;4debug
        If StringLen($export) > 0 Then
            GUICtrlSetData($hiCreate, "GIVICFG:" & $export)
        Else
            GUICtrlSetData($hiCreate, $hiCreateDefaultText)
            If $iItemCount > 0 Then MsgBox($MB_TOPMOST,"Export String","No data to export selected / found",4)
        EndIf
    */
}
