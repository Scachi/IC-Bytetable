#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QRegularExpression>
#include <QDebug>

#include "gpcreader.h"

// tries to open the file : 1. as relative path, 2. as the string is
QString GPCReader::locateFile(QString path) {
    QString file2open=GPCCurrentFilePath;
    // try the selected files directory first (for relative path)
    if (path.length()>0) file2open.prepend(GPCSelectedDir);
    QFile inputFile(file2open);
    if (inputFile.exists()) return file2open;
    // as fallback try the filename as it was found in the #include string (for absolute path)
    file2open=GPCCurrentFilePath;
    return file2open;
}

void GPCReader::addPath(QFile file) {
    if (file.exists()) {
        GPCPathList.append(QFileInfo(file).absolutePath().append("/"));
        GPCPathList.removeDuplicates();
    }
}

void GPCReader::addPath(QString path) {
    QFile inputFile(path);
    if (inputFile.exists()) {
        GPCPathList.append(QFileInfo(GPCCurrentFilePath).absolutePath().append("/"));
        GPCPathList.removeDuplicates();
    }
}

void GPCReader::readFile(QString path) {
    QString file2open=locateFile(path);
    QFile inputFile(file2open);
    if (inputFile.exists()) qDebug() << "Going to read: " << file2open;
    else qDebug() << "Failed to find: " << file2open;
    inputFile.open(QIODevice::ReadOnly);    
    QTextStream in(&inputFile);
    QString line;
    // loop through all lines
    GPCRawList.clear();
    while (!in.atEnd()) {
        GPCRawList.append(in.readLine());
    }

    inputFile.close();
}


void GPCReader::readClipboard() {
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->hasText()) {
        GPCRawList.clear();
        QString sClip=clipboard->text();
        GPCRawList=sClip.split("\n");
        /*
        for (int i = 0; i < GPCRawList.size(); ++i) {
            qDebug() << "GPCRawList: " << GPCRawList[i];
        }
        */
    }

}

void GPCReader::parse() {
    if (GPCCurrentFilePath=="") {
        GPCCurrentFilePath=GPCSelectedFilePath;
        if (GPCSelectedFilePath=="clipboard") {
            readClipboard();
        } else {
            readFile();
        }
    }

    parseGPCRawList();
    parseICRawList();

    while (IncludeList.size()) {
        for (int i = 0; i < IncludeList.size(); ++i) {
            if (!IncludeListDone.contains(IncludeList[i])) {
                IncludeListDone.append(IncludeList[i]);
                GPCCurrentFilePath=IncludeList[i];
                IncludeList.removeAt(i);
                readFile(GPCSelectedDir);
                parseGPCRawList();
                parseICRawList();
            }
        }
    }
}


void GPCReader::parseGPCRawList() {
    findHeaderFiles(GPCRawList);
    //qDebug() << "IncludeList size: " << IncludeList.size();
    /*
    for (int i = 0; i < IncludeList.size(); ++i) {
        qDebug() << "Includes: " << IncludeList[i];
    }
    */
}

// search for lines: #include "files"
void GPCReader::findHeaderFiles(QStringList source) {
    QStringList strlst;
    strlst=GetList(source,"\\s*#include\\s*\"(.*)\".*");
    //qDebug() << "strlst: " << strlst;
    // test if file is already in include list
    for (int i = 0; i < strlst.size(); ++i) {
        //qDebug() << "checking: " << strlst.at(i);
        if (IncludeList.indexOf(strlst.at(i)) == -1 && IncludeListDone.indexOf(strlst.at(i)) == -1) {
            //qDebug() << " Adding to list: " <<
            IncludeList.append(strlst.at(i));
        }
    }
    //qDebug() << "IncludeList: " << IncludeList;
}

bool GPCReader::gpcRawHasIC() {
    QRegularExpression re;
    re.setPattern("<cfgdesc>");
    GPCICBegin=GPCRawList.indexOf(re);
    re.setPattern("</cfgdesc>");
    GPCICEnd=GPCRawList.indexOf(re);
    //qDebug() << "Interactive Configuration: begins at line:" << GPCICBegin << " and ends at line:" << GPCICEnd;
    if (GPCICBegin == -1 || GPCICEnd == -1) {
        qDebug() << "No Interactive Configuration in file " << GPCCurrentFilePath;
        return false;
    }
    return true;
}

void GPCReader::parseICRawList() {
    qDebug() << "parseICRawList";
    if (!gpcRawHasIC()) return;
    qDebug() << "Interactive Configuration found in file " << GPCCurrentFilePath;
    GPCICFound=true;
    ICRawList = GPCRawList.mid(GPCICBegin,GPCICEnd-GPCICBegin);
    //qDebug() << ICRawList;

    QRegularExpression re;
    QRegularExpressionMatch match;
    qint32 icstart,icstop,icnext;

    re.setPattern("^\\s*(\\[.*\\]).*");
    icstart=0; icstop=0; icnext=0;

    // each time read the whole section [SomeSectionName]
    while ((icstart=ICRawList.indexOf(re,icnext)) > -1) {
        icstop=icnext=ICRawList.indexOf(re,icstart+1);
        if (icnext >-1) icstop=icnext-1;
        qDebug() << "Start: " << icstart << " , stop: " << icstop << " , next: " << icnext;
        ICRawSection = ICRawList.mid(icstart,icstop-icstart);

        parseICSection(icstart);
    }
}

// search the section for all keywords and their values
void GPCReader::parseICSection(qint32 line) {
    //ToDo: use a list of linenumbers to check if all "non emtpy lines" got parsed, automark empty one as checked
    qDebug() << "------- ICRAWSECTION ------";
    qDebug() << ICRawSection;
    ICFileName=GPCCurrentFilePath;

    ICLine          =QString::number(line);
    ICName          =GetVal("^\\s*(\\[.*\\]).*");
    ICControl       =GetVal("^\\s*control\\s*=(.*)");
    ICItem          =GetList("^\\s*item\\s*=(.*)");
    ICMinVal        =GetVal("^\\s*minimum\\s*=(.*)");
    ICMaxVal        =GetVal("^\\s*maximum\\s*=(.*)");
    ICStep          =GetVal("^\\s*step\\s*=(.*)");
    ICDecimals      =GetVal("^\\s*decimals\\s*=(.*)");

    ICCollapsible   =GetVal("^\\s*collapsible\\s*=(.*)");
    ICGroup         =GetVal("^\\s*group\\s*=(.*)");
    ICGroupCol      =GetVal("^\\s*groupcol\\s*=(.*)");
    ICColor         =GetVal("^\\s*color\\s*=(.*)");

    ICVarType       =GetVal("^\\s*vartype\\s*=(.*)");
    ICVarName       =GetVal("^\\s*varname\\s*=(.*)");
    ICComment       =GetVal("^\\s*comment\\s*=(.*)");

    ICShortdesc     =GetShortdesc();
    ICByteOffset    =GetVal("^\\s*byteoffset\\s*=(.*)");
    ICBitSize       =GetVal("^\\s*bitsize\\s*=(.*)");
    ICBitOffset     =GetVal("^\\s*bitoffset\\s*=(.*)");
    ICDefaultVal    =GetVal("^\\s*default\\s*=(.*)");

    ICByteOffsetHex =QString("%1").arg(ICByteOffset.toInt(), 2, 16, QLatin1Char( '0' ));
    //ToDo: handle fix32 / fix16
    ICDefaultValHex =QString("%1").arg(ICDefaultVal.toInt(), ICBitSize.toInt()/4, 16, QLatin1Char( '0' ));

    //ICNewVal=
    //ICNewValHex=

    qDebug() << "line: "            << ICLine;
    qDebug() << "name: "            << ICName;
    qDebug() << "control: "         << ICControl;
    qDebug() << "item: "            << ICItem;
    qDebug() << "minval: "          << ICMinVal;
    qDebug() << "maxval: "          << ICMaxVal;
    qDebug() << "step: "            << ICStep;
    qDebug() << "decimals: "        << ICDecimals;
    qDebug() << "collapsible: "     << ICCollapsible;
    qDebug() << "group: "           << ICGroup;
    qDebug() << "groupcol: "        << ICGroupCol;
    qDebug() << "iccolor: "         << ICColor;

    qDebug() << "icvartype: "       << ICVarType;
    qDebug() << "icvarname: "       << ICVarName;
    qDebug() << "iccomment: "       << ICComment;

    qDebug() << "icshortdesc: "     << ICShortdesc;
    qDebug() << "icbyeteoffset: "   << ICByteOffset;
    qDebug() << "icbyteoffhex: "    << ICByteOffsetHex;
    qDebug() << "icbitsize: "       << ICBitSize;
    qDebug() << "icbitoffset: "     << ICBitOffset;
    qDebug() << "icdefaultval: "    << ICDefaultVal;
    qDebug() << "icdefaulthex: "    << ICDefaultValHex;
}

QStringList GPCReader::GetShortdesc() {
    QStringList sval;
    QRegularExpression reMS, reME;
    QRegularExpressionMatch match;
    QString keyS = "^\\s*shortdesc\\s*=(.*)";
    QString keyMS = "^\\s*shortdesc\\s*=\\s*<<<([a-zA-Z]*).*";
    QString keyME;
    qint32 lineMS, lineME;

    reMS.setPattern(keyMS);
    lineMS=ICRawSection.indexOf(reMS);// shortdesc = <<<'MULTILINE' tag

    // multi-line
    if (lineMS > -1) {
        match=reMS.match(ICRawSection[lineMS]);
        keyME="^\\s*(" + match.captured(1) + ")\\s*.*";
        //qDebug() << " multiline identifier: " << match.captured(1);
        //qDebug() << " multiline endsearch key: " << keyME;
        reME.setPattern(keyME);
        lineME=ICRawSection.indexOf(reME);
        //qDebug() << " Multiline Shortdesc start: " << lineMS << " , end: " << lineME;
        sval = ICRawSection.mid(lineMS+1,lineME-lineMS-1);
    } else {
    // single-line
        sval.append(GetVal(keyS,false));
    }
    return sval;
}

QString GPCReader::GetVal(QString key, bool trimmed) {
    return GetVal(ICRawSection,key,trimmed);
}

QString GPCReader::GetVal(QStringList source, QString key, bool trimmed) {
    QString sval;
    QRegularExpression re;
    QRegularExpressionMatch match;

    //qDebug() << " key: " << key;
    re.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);
    re.setPattern(key);
    qint32 line;
    line=source.indexOf(re);
    if (line > -1) {
        //qDebug() << "MATCH !!!!! at " << line;
        match=re.match(source[line]);
        //qDebug() << "Match: " << match;
        sval=match.captured(1);
        if (trimmed) sval=sval.trimmed();
        //qDebug() << "result: " << sval;
        return sval;
    }
    return sval;
}

QStringList GPCReader::GetList(QString key, bool trimmed) {
    return GetList(ICRawSection, key, trimmed);
}

QStringList GPCReader::GetList(QStringList source, QString key, bool trimmed) {
    QString sval;
    QStringList svallist;
    QRegularExpression re;
    QRegularExpressionMatch match;

    //qDebug() << " key: " << key;
    //re.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);
    re.setPattern(key);
    qint32 line=0;

    while ((line=source.indexOf(re,line)) > -1) {
        match=re.match(source[line]);
        sval=match.captured(1);
        if (trimmed) sval=sval.trimmed();
        svallist.append(sval);
        //qDebug() << "result at " << line << " : " << sval;
        line++;
    }
    //qDebug() << "result: " << svallist;
    return svallist;
}
