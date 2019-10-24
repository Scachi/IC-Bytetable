#ifndef XTRA_H
#define XTRA_H

#include <QString>
#include <QBitArray>

// tools for formatting, conversions, ...

class XTRA
{
public:
    XTRA();
    // formatting a decimal value by adding the point to the correct position
    static QString xDecDot (QString value, QString decimals);
    // convert to hex, automatically selects decimal or integer
    static QString x2Hex (QString value, QString bitsize);
    // convert hex hh to bin 00000000, bitsize <= 8 only
    static QString xHex2Bin(QString value, QString bitsize);
    // convert hex to value, automatically selects decimal or integer
    static QString xHex2Val(QString value, QString bitsize, QString bitoffset, QString minimum, QString decimals);
    // convert bin 00000000 to byte
    static QString xBin2Byte(QString value);
    // convert bin 00000000 to hex
    static QString xBin2Hex(QString value);
    // bin OR
    static QString xBinOR(QString value,QString value2);
    // bin AND
    static QString xBinAND(QString value,QString value2);
    // bitarray to string
    static QString xBitArrayToString(QBitArray arr);
    // string to bitarray
    static QBitArray xStringToBitArray(QString str);
    // stop tooltip / richtext from adding automatic linebreaks
    static QString xNoAutoLinebreaks (QString qs);
};

#endif // XTRA_H
