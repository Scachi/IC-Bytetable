#include "ic.h"
#include "gpcreader.h"

IC::IC(QStringList SLRawSection, QString SFileName, QString SLineNo, QString SName, QString SControl)
{
    RawSection=SLRawSection;
    FileName=SFileName;
    LineNo=SLineNo;
    Name=SName;
    Control=SControl;
    Valid=0;
}

IC::IC(QString SFileName, QString SLineNo, QString SName, QString SControl)
{
    FileName=SFileName;
    LineNo=SLineNo;
    Name=SName;
    Control=SControl;
    Valid=0;
}

// qDebug() complete IC
void IC::ICDebug(QVector<IC> ICVec) {
    for(int idx = 0; idx < ICVec.size(); idx++)
    {
        ICOut(ICVec[idx]);
    }
}

// qDebug() specific IC entry by lineNo
void IC::ICDebug(QVector<IC> ICVec, QString sLineNo) {


    for(int idx = 0; idx < ICVec.size(); idx++)
    {
        if( ICVec[idx].LineNo.compare(sLineNo)==0 ) {
            ICOut(ICVec[idx]);
        }
    }

}

void IC::ICOut() {
    this->ICOut(*this);
}

void IC::ICOut(IC ic) {
    qDebug() << "------- ICRAWSECTION ------";
    qDebug() << ic.RawSection;
    qDebug() << "line: "        << ic.LineNo;
    qDebug() << "name: "        << ic.Name;
    qDebug() << "control: "     << ic.Control;
    qDebug() << "item: "        << ic.Item;
    qDebug() << "minval: "      << ic.MinVal;
    qDebug() << "maxval: "      << ic.MaxVal;
    qDebug() << "step: "        << ic.Step;
    qDebug() << "decimals: "    << ic.Decimals;
    qDebug() << "collapsible: " << ic.Collapsible;
    qDebug() << "group: "       << ic.Group;
    qDebug() << "groupcol: "    << ic.GroupCol;
    qDebug() << "iccolor: "     << ic.Color;

    qDebug() << "vartype: "     << ic.VarType;
    qDebug() << "varname: "     << ic.VarName;
    qDebug() << "comment: "     << ic.Comment;

    qDebug() << "shortdesc: "   << ic.Shortdesc;
    qDebug() << "byeteoffset: " << ic.ByteOffset;
    qDebug() << "byteoffhex: "  << ic.ByteOffsetHex;
    qDebug() << "bitsize: "     << ic.BitSize;
    qDebug() << "bitoffset: "   << ic.BitOffset;
    qDebug() << "defaultval: "  << ic.DefaultVal;
    qDebug() << "defaulthex: "  << ic.DefaultValHex;

    qDebug() << "valid: "       << ic.Valid;
    qDebug() << "info: "        << ic.Info;
    qDebug() << "warn: "        << ic.Warn;
    qDebug() << "error: "       << ic.Err;
}
