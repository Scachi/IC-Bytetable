#include "xtra.h"

XTRA::XTRA()
{

}

// formatting a decimal value by adding the point to the correct position
QString XTRA::xDecDot (QString value, QString decimals) {
    if (value.length() < decimals.toInt()+1) value.fill('0',decimals.toInt()+1);
    value.insert(decimals.length() - decimals.toInt()-1,'.');
    return value;
}

// convert to hex, automatically selects decimal or integer
QString XTRA::x2Hex (QString value, QString bitsize) {
    if (value.length() == 0) return "";
    qint32 iconv = value.toInt(); // integer as default (non decimal)
    if (value.contains(".")) { // decimal conversion required
        float fconv = value.toFloat();
        if (bitsize.contains("16")) iconv = qRound(fconv * 256);
        else iconv = qRound(fconv * 65536);
    }
    value = QString("%1").arg(iconv, bitsize.toInt()/4, 16, QLatin1Char( '0' ));
    return value;
}

QString XTRA::xNoAutoLinebreaks (QString qs) {
    qs.prepend("<p style='white-space:pre'>").append("</p>");
    return qs;
}
