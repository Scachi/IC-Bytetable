#include <QRegularExpression>
#include <QTextDocument>

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

QString IC::getFileName() const {
    return QFileInfo(this->fileName).fileName();
}

QString IC::getFileNameFull() const {
    return this->fileName;
}

QString IC::getLineNo() const {
    return this->lineNo;
}

QString IC::getName() const {
    return this->name;
}

QString IC::getNameToolTip() const {
    QString tmp=this->name;
    tmp=tmp.mid(1,tmp.length()-2);
    tmp.prepend(QString("<font color=%1>").arg(this->color)).append("</font>");
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
        case 0: return "FALSE";
        case 1: return "TRUE";
        default: return QString::number(this->valid);
    }
}

QString IC::getInfo() const {
    //return this->info;// << this->warn << this->err;
    QString tmp;
    tmp = "E:" + QString::number(this->err.size()) + " | W:" + QString::number(this->warn.size()) + " | I:" + QString::number(this->info.size());
    return tmp;
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

QString IC::getVarType() const {
    return this->varType;
}

QString IC::getVarName() const {
    return this->varName;
}

QString IC::getComment() const {
    return this->comment;
}
