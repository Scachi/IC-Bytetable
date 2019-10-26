#include <QRegularExpression>
#include <QTextDocument>

#include "ic.h"
#include "gpcreader.h"
#include "xtra.h"

IC::IC(QStringList slRawSection, QString sFileName, QString sLineNo, QString sName, QString sControl)
{
    rawSection = slRawSection;
    fileName.append(sFileName);
    lineNo.append(sLineNo);
    name = sName;
    control = sControl;
    valid = 0;
}

IC::IC(QString sFileName, QString sLineNo, QString sName, QString sControl)
{
    fileName.append(sFileName);
    lineNo.append(sLineNo);
    name = sName;
    control = sControl;
    valid = 0;
}

IC::~IC() {

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
/*
    qDebug() << "vartype: "     << this->varType;
    qDebug() << "varname: "     << this->varName;
    qDebug() << "comment: "     << this->comment;
*/
    qDebug() << "shortdesc: "   << this->shortDesc;
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

QString IC::getFileNameFirst() const {
     return QFileInfo(this->fileName[0]).fileName();
}

QString IC::getFileName() const {
    QString filenames;
    for (int i=0; i < this->fileName.size();i++)
    {
        if (i > 0) filenames.append("\n");
        filenames.append(QFileInfo(this->fileName[i]).fileName());
    }
    return filenames;
}

QString IC::getFileNameFull() const {
    return this->fileName.join("\n");
}

QString IC::getFileNameLineNo() const {
    QString filenames;

    for (int i=0; i < this->fileName.size();i++)
    {
        if (i > 0) filenames.append("\n");
        filenames.append(QFileInfo(this->fileName[i]).fileName());
        filenames.append(":").append(this->lineNo[i]);
    }
    return filenames;

}

QString IC::getFileNameFullLineNo() const {
    QString filenames;

    for (int i=0; i < this->fileName.size();i++)
    {
        if (i > 0) filenames.append("\n");
        filenames.append(this->fileName[i]);
        filenames.append(":").append(this->lineNo[i]);
    }
    return filenames;

}

QString IC::getLineNoFirst() const {
     return this->lineNo[0];
}

QString IC::getLineNo() const {
    return this->lineNo.join("\n");
}

QString IC::getLineNoCount() const {
    return QString::number(this->lineNo.size());
}

QString IC::getName() const {
    return this->name;
}


QString IC::getNameToolTip() const {
    QString tmp=this->name;
    tmp=tmp.mid(1,tmp.length()-2);
    tmp.prepend(QString("<font color=%1>").arg(this->color)).append("</font>");
    if (getCollapsible().toInt()==3) tmp.prepend("<i>hidden: ").append("</i>");
    if (getGroup().length() || getGroupCol().length()) tmp.prepend("<i>grouped: ").append("</i>");
    tmp.prepend("<p style='white-space:pre'>").append("</p>"); // linebreaks as in text
    return tmp;
}

QString IC::getByteOffset() const {
    return this->byteOffset;
}

QString IC::getByteOffsetHex() const {
    return this->byteOffsetHex;
}

QString IC::getBitSize() const {
    return this->bitSize;
}

QString IC::getBitOffset() const {
    return this->bitOffset;
}

QString IC::getValid() const {
    switch (this->valid) {
        case 0:
        case 1: return "OK";
        case 2: return "INFO";
        case 4: return "WARN";
        case 8: return "ERROR";
        default: return QString::number(this->valid);
    }
}

QString IC::getInfoCode() const {
    //return this->info;// << this->warn << this->err;
    QString tmp;
    tmp = "E:" + QString::number(this->err.size()) + " | W:" + QString::number(this->warn.size()) + " | I:" + QString::number(this->info.size());
    return tmp;
}

QString IC::getInfo() const {
    //return this->info;// << this->warn << this->err;
    QString tmp;
    if (this->err.size() > 0)
    {
        tmp.append(this->err.join("\n"));
    }
    if (this->warn.size() > 0)
    {
        tmp.append(this->warn.join("\n"));
    }
    if (this->info.size() > 0)
    {
        tmp.append(this->info.join("\n"));
    }
    return tmp;
}

QString IC::getInfoToolTip() const {
    //return this->info;// << this->warn << this->err;
    QString tmp;
    if (this->err.size() > 0)
    {
        tmp.append("<font color='#c70202'>Error:</font>\n");
        tmp.append(this->err.join("\n"));
    }
    if (this->warn.size() > 0)
    {
        if (tmp.size()>0) tmp.append("\n");
        tmp.append("<font color='#f79a05'>Warn:</font>\n");
        tmp.append(this->warn.join("\n"));
    }
    if (this->info.size() > 0)
    {
        if (tmp.size()>0) tmp.append("\n");
        tmp.append("<font color='#02a1db'>Info:</font>\n");
        tmp.append(this->info.join("\n"));
    }
    if (tmp.length()>0) return XTRA::xNoAutoLinebreaks(tmp);
    return {};
}


QString IC::getDefaultVal() const {
    return this->defaultVal;
}

QString IC::getDefaultValHex() const {
    return this->defaultValHex;
}

QString IC::getNewVal() const {
    return this->newVal;
}

QString IC::getNewValHex() const {
    return this->newValHex;
}

QString IC::getControl() const {
    return this->control;
}

QString IC::getItemCount(QString prefix) const {
    if (!this->item.size()) return {};
    return QString::number(this->item.size()).prepend(prefix);
}

QString IC::getItemNamesToolTip() const {
    if (!this->item.size()) return {};
    return this->item.join("</li><li>").prepend("<ul type='square'><li>").append("</li></ul>").prepend("<p style='white-space:pre'>").append("</p>");
}

QString IC::getMinVal() const {
    return this->minVal;
}

QString IC::getMaxVal() const {
    return this->maxVal;
}

QString IC::getDecimals() const {
    return this->decimals;
}

QString IC::getStep() const {
    return this->step;
}

QString IC::getShortDesc(int limit) const {
    QString tmpQStr;
    for (int i=0;i<this->shortDesc.size();i++)
    {
        if (this->shortDesc[i].length() > 0)
        {
            tmpQStr.append(this->shortDesc[i]);

            // add linebreak only when it is not the last line and
            // it has no code tags that do linebreaks on their own
            if (!containsBRTags(this->shortDesc[i])
                    && i < this->shortDesc.size()-1)
            {
                tmpQStr.append("<br>");
            }

            if (i == limit)
            {
                if (i < this->shortDesc.size()-1) {
                    tmpQStr.append("<br><b>. . . </b><i>not all content is shown</i> <b>. . . </b>");
                }
                break;
            }
        }
    }
    if (tmpQStr.length() > 0 ) return tmpQStr.prepend("<p style='white-space:pre'>").append("</p>");
    return {};
}

QString IC::getShortDescPlain(int limit) const {
    QTextDocument doc;
    QString shortdesc=getShortDesc(limit);
    if (shortdesc.length() > 0)
    {
        doc.setHtml(shortdesc);
        shortdesc = doc.toPlainText();
        shortdesc=shortdesc.trimmed();
        shortdesc.replace("\n:",":"); // remove linebreaks in front of :
        return shortdesc;
    }
    return {};
}

QString IC::getCollapsible() const {
    return this->collapsible;
}

QString IC::getCollapsibleToolTip() const {
    if (this->collapsible.length()==0) return {};
    switch(this->collapsible.toInt())
    {
        case  1: return "unfolded";
        case  2: return "folded";
        case  3: return "hidden";
        default: return {};
    }
}

QString IC::getGroup() const {
    return this->group;
}

QString IC::getGroupCol() const {
    return this->groupCol;
}

QString IC::getColor() const {
    return this->color;
}

QString IC::getColorToolTip() const {
    if (this->color.length()==0) return {};
    QString tmp=this->color;
    tmp.prepend(QString("<font color=%1>").arg(this->color)).append("</font>");
    return tmp;
}

QString IC::getBorder() const {
    return this->border;
}
/*
QString IC::getVarType() const {
    return this->varType;
}

QString IC::getVarName() const {
    return this->varName;
}

QString IC::getComment() const {
    return this->comment;
}
*/

bool IC::setNewVal(QString newval)
{
    this->newVal=newval;
    if (this->newVal.length() > 0 && this->decimals.length() > 0 && !newval.contains("."))
    {
        this->newVal.append(".").append(QString("%1").arg(0,this->decimals.toInt(),16,QLatin1Char('0')));
    }
    return true;
}

bool IC::containsBRTags(QString source) const {
    QRegularExpression re;
    QRegularExpressionMatch match;
    QString key = ".*<\\s*\\\\*(code|div|h1|h2|h3|h4|h5|h6|ol|p|table|tbody|td|tfoot|th|thead|tr|ul)\\s*>.*";

    re.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);
    re.setPattern(key);
    qint32 line;
    line = source.indexOf(re);
    if (line > -1)
    {
        return true;
    }
    return false;
}

qint8 IC::reValidate()
{
    this->err.clear();
    this->warn.clear();
    this->info.clear();
    this->valid=0;
    return validate();
}

qint8 IC::validate()
{
    validateControl();

    if (this->group.length() > 0 || this->groupCol.length() > 0)
    {
        if (this->color.length()>0) this->infoAdd("'color' has no effect on grouped controls");
    }

    return this->valid;
}

qint8 IC::validateControl()
{
    if (this->control.length() == 0) return false;
    if (this->control == "info" ) return true;
    if (this->control == "space" )
    {
        validateControlDefault();
    }

    if (this->control == "config")
    {
        validateControlItem(false);
    }

    if (this->control == "checkbox" ||
        this->control == "radiobox" ||
        this->control == "combobox" )
    {
        validateControlCommon();
        validateControlItem();
    }

    if (this->control == "spinbox"  ||
        this->control == "spinboxf" ||
        this->control == "slider"   ||
        this->control == "dial"     )
    {
        validateControlCommon();
        validateControlValue();

    }

    return false;
}

qint8 IC::validateControlDefault()
{
    // default
    if (this->defaultVal.length() > 0) return this->valid;
    this->warnAdd("'default' missing");
    return this->valid;
}

qint8 IC::validateControlCommon()
{
    // byteoffset, "bitsize", "bitoffset"
    validateByteOffset();
    validateBitSize();
    if (this->bitSize.toInt()<8) validateBitOffset();
    //validateControlDefault();
    return this->valid;
}

qint8 IC::validateControlItem(bool checkdefault)
{
    if (this->item.size()==0) errAdd("'item's missing");
    if (checkdefault) {
        if (this->defaultVal.length() == 0) this->warnAdd("'default' missing");
        else if (this->defaultVal.toInt() < 0 || this->defaultVal.toInt() > this->item.size())
            warnAdd("'default' value invalid");
    }
    return this->valid;
}

qint8 IC::validateControlValue()
{
    // mimimum, maximum, step
    if (this->minVal.length()==0) errAdd("'minimum' missing");
    if (this->maxVal.length()==0) errAdd("'maximum' missing");
    if (this->step.length()==0) errAdd("'step' missing");
    if (this->defaultVal.length() == 0) this->warnAdd("'default' missing");
    else if (this->defaultVal.toFloat() < this->minVal.toFloat() ||
        this->defaultVal.toFloat() > this->maxVal.toFloat())
    {
        warnAdd("'default' value invalid");
    }
    if (this->minVal.toFloat() > this->maxVal.toFloat())
        errAdd("'minimum' value exceeds maximum value");
    if (this->maxVal.toFloat() < this->minVal.toFloat())
        errAdd("'maximum' value is lower than minimum value");

    if (this->newVal.length()==0) return this->valid;

    if (this->newVal.toFloat() < this->minVal.toFloat())
    {
        errAdd("value is lower than minimum value");
    }
    if (this->newVal.toFloat() > this->maxVal.toFloat())
    {
        errAdd("value exceeds maximum value");
    }
    return this->valid;
}


qint8 IC::validateByteOffset()
{
    if (this->byteOffset.length() >0 ) {
        if (this->byteOffset.toInt() < 0 ||
            this->byteOffset.toInt() > 127 ||
            this->byteOffset.toInt() + this->bitSize.toInt()/8 > 128
        ) {
            this->errAdd("'byteoffset' value invalid");
        }
        return this->valid;
    }
    this->errAdd("'byteoffset' missing");
    return this->valid;
}

qint8 IC::validateBitSize()
{
    if (this->bitSize.length() >0 ) {
        qint32 bs = this->bitSize.toInt();
        if (bs < 0 || bs > 32 ||
            (bs > 7 && (bs%8 != 0))
        ) {
            this->errAdd("'bitsize' invalid");
        }
        return this->valid;
    }
    this->errAdd("'bitsize' missing");
    return this->valid;
}

qint8 IC::validateBitOffset()
{
    if (this->bitOffset.length() >0 ) {
        if (this->bitOffset.toInt() < 0 ||
            this->bitOffset.toInt() > 7)
        {
            this->errAdd("'bitoffset' value invalid");
        }
        if (this->bitOffset.toInt() + this->bitSize.toInt() > 8)
        {
            this->errAdd("'bitoffset+bitsize' > 7");
        }
        return this->valid;
    }
    this->errAdd("'bitoffset' missing");
    return this->valid;
}

qint8 IC::infoAdd(QString msg)
{
    //qDebug() << "info!!!";
    this->info.append(msg);
    if (this->valid < 2) this->valid=2;
    return this->valid;
}

qint8 IC::warnAdd(QString msg)
{
    //qDebug() << "warn!!!";
    this->warn.append(msg);
    if (this->valid < 4) this->valid=4;
    return this->valid;
}

qint8 IC::errAdd(QString msg)
{
    //qDebug() << "err!!!";
    this->err.append(msg);
    if (this->valid < 8) this->valid=8;
    return this->valid;
}

qint8 IC::errAdd(QStringList msg)
{
    //qDebug() << "err!!!";
    this->err.append(msg);
    if (this->valid < 8) this->valid=8;
    return this->valid;
}


qint8 IC::msgAdd(QString msg, qint8 severity)
{
    switch(severity)
    {
        case 2 : return infoAdd(msg);
        case 4 : return warnAdd(msg);
        default : return errAdd(msg);
    }
}

bool IC::isUsingPMEM()
{
    if (this->byteOffset.length()>0) return true;

    return false;
}

bool IC::isChecked() const
{
    return this->checked;
}

bool IC::getSize(int *bytes, int *bits)
{
    if (this->isUsingPMEM()) {
        int bitsize = 8;
        if (this->getBitSize().length()>0) bitsize = this->getBitSize().toInt();

        if (this->getBitOffset().length() > 0 || bitsize<8) *bits+=bitsize;
        else *bytes+=bitsize/8;
        return true;
    }
    return false;
}

int IC::canMerge(IC ic)
{
    if ( this->control.length() > 0 && ic.control.length() > 0 ) return 1;
    if ( this->minVal.length() > 0 && ic.minVal.length() > 0 ) return 2;
    if ( this->maxVal.length() > 0 && ic.maxVal.length() > 0 ) return 3;
    if ( this->step.length() > 0 && ic.step.length() > 0 ) return 4;
    if ( this->decimals.length() > 0 && ic.decimals.length() > 0 ) return 5;

    if ( this->collapsible.length() > 0 && ic.collapsible.length() > 0 ) return 6;
    if ( this->group.length() > 0 && ic.group.length() > 0 ) return 7;
    if ( this->groupCol.length() > 0 && ic.groupCol.length() > 0 ) return 8;
    if ( this->color.length() > 0 && ic.color.length() > 0 ) return 9;
    if ( this->border.length() > 0 && ic.border.length() > 0 ) return 10;

    if ( this->shortDesc.join("").length() > 0 && ic.shortDesc.join("").length() > 0 ) return 11;
    if ( this->byteOffset.length() > 0 && ic.byteOffset.length() > 0 ) return 12;
    if ( this->byteOffsetHex.length() > 0 && ic.byteOffsetHex.length() > 0 ) return 13;
    if ( this->bitSize.length() > 0 && ic.bitSize.length() > 0 ) return 14;
    if ( this->bitOffset.length() > 0 && ic.bitOffset.length() > 0 ) return 15;
    if ( this->defaultVal.length() > 0 && ic.defaultVal.length() > 0 ) return 16;
    if ( this->defaultValHex.length() > 0 && ic.defaultValHex.length() > 0 ) return 17;

    return 0;
}

bool IC::merge(IC ic)
{
    this->lineNo.append(ic.lineNo);
    this->fileName.append(ic.fileName);
    if ( this->control.length() == 0) this->control=ic.control;
    if (ic.item.join("").length() > 0) this->item.append(ic.item);
    if ( this->minVal.length() == 0) this->minVal=ic.minVal;
    if ( this->maxVal.length() == 0) this->maxVal=ic.maxVal;
    if ( this->step.length() == 0) this->step=ic.step;
    if ( this->decimals.length() == 0) this->decimals=ic.decimals;

    if ( this->collapsible.length() == 0) this->collapsible=ic.collapsible;
    if ( this->group.length() == 0) this->group=ic.group;
    if ( this->groupCol.length() == 0) this->groupCol=ic.groupCol;
    if ( this->color.length() == 0) this->color=ic.color;
    if ( this->border.length() == 0) this->border=ic.border;

    if ( this->shortDesc.join("").length() == 0) this->shortDesc=ic.shortDesc;
    if ( this->byteOffset.length() == 0) this->byteOffset=ic.byteOffset;
    if ( this->byteOffsetHex.length() == 0) this->byteOffsetHex=ic.byteOffsetHex;
    if ( this->bitSize.length() == 0) this->bitSize=ic.bitSize;
    if ( this->bitOffset.length() == 0) this->bitOffset=ic.bitOffset;
    if ( this->defaultVal.length() == 0) this->defaultVal=ic.defaultVal;
    if ( this->defaultValHex.length() == 0) this->defaultValHex=ic.defaultValHex;

    return true;
}

void IC::newValToHex()
{
    this->newValHex = XTRA::x2Hex(this->newVal,this->bitSize);
    this->reValidate();
}

void IC::newHexToVal()
{
    this->newVal = XTRA::xHex2Val(this->newValHex,this->bitSize,this->bitOffset,this->minVal,this->decimals,this->control);
    this->reValidate();
}
