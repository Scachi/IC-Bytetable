#include "ic.h"
#include "gpcreader.h"

IC::IC(QStringList slRawSection, QString sFileName, QString sLineNo, QString sName, QString sControl)
{
    rawSection = slRawSection;
    fileName = sFileName;
    lineNo = sLineNo;
    name = sName;
    control = sControl;
    valid = 0;
}

IC::IC(QString sFileName, QString sLineNo, QString sName, QString sControl)
{
    fileName = sFileName;
    lineNo = sLineNo;
    name = sName;
    control = sControl;
    valid = 0;
}


void IC::debug() {
    qDebug() << "------- ICRAWSECTION ------";
    qDebug() << this->rawSection;
    qDebug() << "line: "        << this->lineNo;
    qDebug() << "name: "        << this->name;
    qDebug() << "control: "     << this->control;
    qDebug() << "item: "        << this->item;
    qDebug() << "minval: "      << this->minVal;
    qDebug() << "maxval: "      << this->maxVal;
    qDebug() << "step: "        << this->step;
    qDebug() << "decimals: "    << this->decimals;
    qDebug() << "collapsible: " << this->collapsible;
    qDebug() << "group: "       << this->group;
    qDebug() << "groupcol: "    << this->groupCol;
    qDebug() << "iccolor: "     << this->color;

    qDebug() << "vartype: "     << this->varType;
    qDebug() << "varname: "     << this->varName;
    qDebug() << "comment: "     << this->comment;

    qDebug() << "shortdesc: "   << this->shortdesc;
    qDebug() << "byeteoffset: " << this->byteOffset;
    qDebug() << "byteoffhex: "  << this->byteOffsetHex;
    qDebug() << "bitsize: "     << this->bitSize;
    qDebug() << "bitoffset: "   << this->bitOffset;
    qDebug() << "defaultval: "  << this->defaultVal;
    qDebug() << "defaulthex: "  << this->defaultValHex;

    qDebug() << "valid: "       << this->valid;
    qDebug() << "info: "        << this->info;
    qDebug() << "warn: "        << this->warn;
    qDebug() << "error: "       << this->err;
}
