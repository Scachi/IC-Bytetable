#ifndef XTRA_H
#define XTRA_H

#include <QString>

// tools for formatting, conversions, ...

class XTRA
{
public:
    XTRA();
    // formatting a decimal value by adding the point to the correct position
    static QString xDecDot (QString value, QString decimals);
    // convert to hex, automatically selects decimal or integer
    static QString x2Hex (QString value, QString bitsize);
    // stop tooltip / richtext from adding automatic linebreaks
    static QString xNoAutoLinebreaks (QString qs);
};

#endif // XTRA_H
