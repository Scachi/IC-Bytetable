#include <QDebug>
#include <QBitArray>
#include "xtra.h"

XTRA::XTRA()
{

}

// formatting a decimal value by adding the point to the correct position
QString XTRA::xDecDot(QString value, QString decimals)
{
    if (value.length() < decimals.toInt()+1) value.fill('0',decimals.toInt()+1);
    value.insert(decimals.length() - decimals.toInt()-1,'.');
    return value;
}

// convert to hex, automatically selects decimal or integer
QString XTRA::x2Hex(QString value, QString bitsize)
{
    if (value.length() == 0) return "";
    qint32 iconv = value.toInt(); // integer as default (non decimal)
    if (value.contains(".")) { // decimal conversion required
        float fconv = value.toFloat();
        if (bitsize.contains("16")) iconv = qRound(fconv * 256);
        else iconv = qRound(fconv * 65536);
    }
    if (bitsize.toInt() < 8) bitsize="8";
    value = QString("%1").arg(iconv, bitsize.toInt()/4, 16, QLatin1Char('0'));
    if (iconv < 0) value = value.right(bitsize.toInt()/4);
/*
    qDebug() << " x2Hex: " << value << " bitsize.toInt()/4: " << bitsize.toInt()/4 <<
                " iconv: " << iconv;
*/
    return value.toUpper();
}

// convert to hex, automatically selects decimal or integer
QString XTRA::xHex2Val(QString value, QString bitsize, QString bitoffset, QString minimum, QString decimals)
{
    if (value.length() == 0) return "";

    int bitSize = 0;
    if (bitsize.length() > 0) bitSize=bitsize.toInt();

    int decimal = 0;
    if (decimals.length() > 0) decimal = decimals.toInt();

    int bitOffset = -1;
    if (bitoffset.length() >0 ) bitOffset = bitoffset.toInt();

    bool ok;
    double number = value.toUInt(&ok,16);
    QString sNumber = QString::number(number);

    if (bitOffset >= 0 || bitSize < 8) // only return bits of the number
    {
        QString sBin = xHex2Bin(value,bitsize);
        QString sBinBits = sBin.mid(8 - (bitOffset+bitSize), bitSize);
        sNumber = xBin2Byte(sBinBits);
        qDebug() << "sBin: " << sBin << " , boff: " << bitOffset << " , bsize: " << bitSize << " , sBinBits: " << sBinBits << " , sNumber: "<<  sNumber;
    }
    else if (decimal)
    {
        if (bitSize == 16) sNumber = QString::number(number / 256, 'f', decimal);
        if (bitSize == 32) sNumber = QString::number(number / 65536, 'f', decimal);
    }
    if (minimum.toInt() < 0)
    {
        QString sHex = x2Hex((QString::number(sNumber.toInt()-1)),bitsize);
        QString sBin = xHex2Bin(sHex,bitsize);
        //qDebug() << "value: " << sNumber << " , bin: " << sBin;
        if (sBin.left(1) == "1") {
            for (int i=0; i<sBin.length();i++)
            {
                if (sBin[i]=='1') sBin[i]='0';
                else sBin[i]='1';
            }
            //qDebug() << "now value: " << xBin2Byte(sBin).toInt() << " , bin:" << sBin;
            sNumber = xBin2Byte(sBin);
            sNumber = QString::number(sNumber.toInt()*-1);
        }
    }

    //qDebug() << "hex2val hex: " << value << " | number: " << sNumber;
    return sNumber;
}

// convert hex hh to bin 00000000
QString XTRA::xHex2Bin(QString value, QString bitsize)
{
    if (value.length() == 0 || bitsize.toInt() > 8) return "";
    bool ok;
    QString sBin= QString("%1").arg(value.toULongLong(&ok, 16), 8, 2, QChar('0'));
    //qDebug() << "src: " << value << "bin: " << sBin;
    return sBin;
}

QString XTRA::xBin2Hex(QString value)
{
    QString sHex= xBin2Byte(value);
    sHex = x2Hex(sHex,"8");
    return sHex;
}

// convert bin to byte
QString XTRA::xBin2Byte(QString value)
{
    if (value.length() == 0) return "";
    bool ok;

    QString sByte = QString::number(value.toInt(&ok,2));
    return sByte;
}

QString XTRA::xBinOR(QString value,QString value2)
{
    QBitArray bAR(value.size());
    QBitArray bA1 = xStringToBitArray(value);
    QBitArray bA2 = xStringToBitArray(value2);
    bAR = bA1 | bA2;
    /*
    qDebug() << "a1: " << bA1;
    qDebug() << "a2: " << bA2;
    qDebug() << "ar: " << bAR;
    */
    return xBitArrayToString(bAR);
}

QString XTRA::xBinAND(QString value,QString value2)
{
    QBitArray bAR(value.size());
    QBitArray bA1 = xStringToBitArray(value);
    QBitArray bA2 = xStringToBitArray(value2);
    bAR = bA1 & bA2;
    /*
    qDebug() << "a1: " << bA1;
    qDebug() << "a2: " << bA2;
    qDebug() << "ar: " << bAR;
    */
    return xBitArrayToString(bAR);
}

QString XTRA::xBitArrayToString(QBitArray arr)
{
    QString arrStr = "";
    for(int i = 0; i<arr.size(); i++)
        arrStr += arr.at(i) ? '1' : '0';
    return arrStr;
}

QBitArray XTRA::xStringToBitArray(QString str)
{
    QBitArray bA(str.length());

    for(int i = 0; i<str.size(); i++)
    {
        if (str[i] == '0') bA[i]=0;
        else bA[i]=1;
    }
    return bA;
}

QString XTRA::xNoAutoLinebreaks(QString qs)
{
    qs.prepend("<p style='white-space:pre'>").append("</p>");
    return qs;
}
