#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QRegularExpression>
#include <QDebug>

#include "ic.h"
#include "icd.h"
#include "gpcreader.h"
#include "xtra.h"
#include "pmem.h"
#include "pmemd.h"

GPCReader::GPCReader(QString sDir, QString sFilePath) {
       icData = new ICD; // parsed interactive configuration data
       pmemData = new PMEMD; // IC byte usage 0..127
       gpcSelectedDir = sDir;
       gpcSelectedFilePath = sFilePath;
       gpcCurrentFilePath = "";
       gpcValid=true;
       //qDebug() << " Sdir / sfilepath: " << sDir << " / " << sFilePath;
       parse();
}

bool GPCReader::getGPCICFound() {
    return this->gpcICFound;
}

bool GPCReader::isValid() {
    return this->gpcValid;
}

void GPCReader::parse() {
    if (gpcCurrentFilePath == "") {
        gpcCurrentFilePath = gpcSelectedFilePath;
        if (gpcSelectedFilePath == "clipboard")
        {
            readClipboard();
        } else readFile();
    }

    parseGPCRawList();
    parseICRawList();
    while (includeList.size())
    {
        for (int i = 0; i < includeList.size(); ++i)
        {
            if (!includeListDone.contains(includeList[i]))
            {
                includeListDone.append(includeList[i]);
                gpcCurrentFilePath = includeList[i];
                includeList.removeAt(i);
                readFile(gpcSelectedDir);
                parseGPCRawList(); // search for #include lines
                parseICRawList();
            }
        }
    }
    icData->bits2Hex();
    if (!icData->validateUniqueNames())
    {
        this->gpcValid=false;
        qDebug("Not unique!");
    }

    if (!pmemData->update(icData)) { // fill PMEM data (filename,lineno,bytes,bits,offsets)
        this->gpcValid=false;
        qDebug("pmemData not valid");
    }
}

void GPCReader::readFile(QString path) {
    QString file2open = locateFile(path);
    QFile inputFile(file2open);
    /*
    if (inputFile.exists()) qDebug() << "Going to read: " << file2open;
    else qDebug() << "Failed to find: " << file2open;
    */

    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    QString line;
    // loop through all lines
    gpcRawList.clear();

    while (!in.atEnd())
    {
        gpcRawList.append(in.readLine());
    }
    inputFile.close();
}


void GPCReader::readClipboard() {
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->hasText())
    {
        gpcRawList.clear();
        QString sClip = clipboard->text();
        gpcRawList = sClip.split("\n");
        /*
        for (int i = 0; i < GPCRawList.size(); ++i) {+
            qDebug() << "GPCRawList: " << GPCRawList[i];
        }
        */
    }
}

// tries to open the file : 1. as relative path, 2. as the string is
QString GPCReader::locateFile(QString path) {
    QString file2open = gpcCurrentFilePath;
    // try the selected files directory first (for relative path)
    if (path.length()>0) file2open.prepend(gpcSelectedDir);
    QFile inputFile(file2open);
    if (inputFile.exists()) return file2open;
    // as fallback try the filename as it was found in the #include string (for absolute path)
    file2open = gpcCurrentFilePath;
    return file2open;
}

void GPCReader::addPath(QFile file) {
    if (file.exists())
    {
        gpcPathList.append(QFileInfo(file).absolutePath().append("/"));
        gpcPathList.removeDuplicates();
    }
}

void GPCReader::addPath(QString path) {
    QFile inputFile(path);
    if (inputFile.exists())
    {
        gpcPathList.append(QFileInfo(gpcCurrentFilePath).absolutePath().append("/"));
        gpcPathList.removeDuplicates();
    }
}

void GPCReader::parseGPCRawList() {
    findHeaderFiles(gpcRawList);
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
    strlst = getList(source,"\\s*#include\\s*\"(.*)\".*");
    //qDebug() << "strlst: " << strlst;
    // test if file is already in include list
    for (int i = 0; i < strlst.size(); ++i)
    {
        //qDebug() << "checking: " << strlst.at(i);
        if (includeList.indexOf(strlst.at(i)) == -1 &&
            includeListDone.indexOf(strlst.at(i)) == -1)
        {
            //qDebug() << " Adding to list: " <<
            includeList.append(strlst.at(i));
        }
    }
    //qDebug() << "IncludeList: " << IncludeList;
}

bool GPCReader::gpcRawHasIC() {
    QRegularExpression re;
    re.setPattern(".*<cfgdesc>.*");
    gpcICBegin = gpcRawList.indexOf(re);
    re.setPattern(".*</cfgdesc>.*");
    gpcICEnd = gpcRawList.indexOf(re);
    //qDebug() << "Interactive Configuration: begins at line:" << gpcICBegin << " and ends at line:" << gpcICEnd;
    if (gpcICBegin == -1 || gpcICEnd == -1)
    {
        //qDebug() << "No Interactive Configuration in file " << gpcCurrentFilePath;
        return false;
    }
    return true;
}

void GPCReader::parseICRawList() {
    //qDebug() << "parseICRawList";
    if (gpcCurrentFilePath.endsWith(".gcd")) // cached IC content in GTunerIV\cache
    {
        gpcICFound = true;
        icRawList = gpcRawList;
        gpcICBegin = 0;
    }
    else // regular .gpc .gph files
    {
        if (!gpcRawHasIC()) {
            return;
        }
        //qDebug() << "Interactive Configuration found in file " << gpcCurrentFilePath;
        gpcICFound = true;
        icRawList = gpcRawList.mid(gpcICBegin, gpcICEnd-gpcICBegin);
    }
    //qDebug() << ICRawList;

    QRegularExpression re;
    QRegularExpressionMatch match;
    qint32 icstart, icstop, icnext;

    re.setPattern("^\\s*(\\[.*\\]).*");
    icstart = 0; icstop = 0; icnext = 0;

    // each time read the whole section [SomeSectionName]
    while ((icstart = icRawList.indexOf(re,icnext)) > -1)
    {
        icstop = icnext = icRawList.indexOf(re,icstart+1);
        if (icnext > -1) icstop = icnext-1;
        //qDebug() << "Start: " << icstart << " , stop: " << icstop << " , next: " << icnext;
        icRawSection = icRawList.mid(icstart, icstop-icstart+1);
        parseICSection(icstart);
    }
}

// search the section for all keywords and their values
void GPCReader::parseICSection(qint32 line) {
    //ToDo: use a list of linenumbers to check if all "non emtpy lines" got parsed, automark empty one as checked
    //qDebug() << "------- ICRAWSECTION ------";
    //qDebug() << ICRawSection;
    icFileName = gpcCurrentFilePath;

    icLineNo    = QString::number(line+gpcICBegin+1);
    icName      = getVal("^\\s*(\\[.*\\]).*");
    icControl   = getVal("^\\s*control\\s*=(.*)");

    IC newIC = IC(icRawSection, icFileName, icLineNo, icName, icControl);

    newIC.item          = getList("^\\s*item\\s*=(.*)");
    newIC.maxVal        = getVal("^\\s*maximum\\s*=\\s*(-*[0-9]*).*");
    newIC.minVal        = getVal("^\\s*minimum\\s*=\\s*(-*[0-9]*).*");
    newIC.step          = getVal("^\\s*step\\s*=\\s*([0-9]*).*");
    newIC.decimals      = getVal("^\\s*decimals\\s*=\\s*([0-9]*).*");

    newIC.collapsible   = getVal("^\\s*collapsible\\s*=\\s*([0-9]*).*");
    newIC.group         = getVal("^\\s*group\\s*=\\s*([01]|true|false).*");
    newIC.groupCol      = getVal("^\\s*groupcol\\s*=\\s*([01]|true|false).*");
    newIC.color         = getVal("^\\s*color\\s*=(.*)");
    newIC.border        = getVal("^\\s*border\\s*=\\s*([0-9]*).*");
/*
    newIC.varType       = getVal("^\\s*vartype\\s*=(.*)");
    newIC.varName       = getVal("^\\s*varname\\s*=(.*)");
    newIC.comment       = getVal("^\\s*comment\\s*=(.*)");
*/
    newIC.shortDesc     = getShortDesc();
    newIC.byteOffset    = getVal("^\\s*byteoffset\\s*=\\s*([0-9]*).*");
    newIC.bitSize       = getVal("^\\s*bitsize\\s*=\\s*([0-9]*).*");
    newIC.bitOffset     = getVal("^\\s*bitoffset\\s*=\\s*([0-9]*).*");
    newIC.defaultVal    = getVal("^\\s*default\\s*=\\s*(-*[0-9]*).*");

    // insert dot at decimal position
    if (newIC.control.indexOf("spinboxf") > -1)
    {
        // formatting the decimal values
        newIC.minVal = XTRA::xDecDot(newIC.minVal, newIC.decimals);
        newIC.maxVal = XTRA::xDecDot(newIC.maxVal, newIC.decimals);
        newIC.defaultVal = XTRA::xDecDot(newIC.defaultVal, newIC.decimals);
        newIC.step = XTRA::xDecDot(newIC.step, newIC.decimals);
    }
    newIC.defaultValHex = XTRA::x2Hex(newIC.defaultVal,newIC.bitSize);

    newIC.byteOffsetHex = XTRA::x2Hex(newIC.byteOffset,"8");

    // checked state
    newIC.checked=false;

    //newIC.debug();
    newIC.validate();

    //qDebug() << "Size of List: " << icData->data.size();
    //icData->data.append(newIC);
    icData->add(newIC); // append or merge entry
    icData->bitsUsed += newIC.bitSize.toInt();

    //qDebug() << "Size of List: " << icData->data.size();
    /*
    //ICNewVal =
    //ICNewValHex =
    */
}

QStringList GPCReader::getShortDesc() {
    QStringList sval;
    QRegularExpression reMS, reME;
    QRegularExpressionMatch match;
    QString keyS = "^\\s*shortdesc\\s*=(.*)";
    QString keyMS = "^\\s*shortdesc\\s*=\\s*<<<([a-zA-Z]*).*"; // get the real used MULTILINE tag
    QString keyME;
    qint32 lineMS, lineME;

    reMS.setPattern(keyMS);
    lineMS = icRawSection.indexOf(reMS);// shortdesc = <<<'MULTILINE' tag

    // multi-line
    if (lineMS > -1)
    {
        match = reMS.match(icRawSection[lineMS]);
        keyME = "^\\s*(" + match.captured(1) + ")\\s*.*";
        //qDebug() << " multiline identifier: " << match.captured(1);
        //qDebug() << " multiline endsearch key: " << keyME;
        reME.setPattern(keyME);
        lineME = icRawSection.indexOf(reME);
        //qDebug() << " Multiline shortdesc start: " << lineMS << " , end: " << lineME;
        sval = icRawSection.mid(lineMS+1, lineME-lineMS-1);
    } else {
    // single-line
        sval.append(getVal(keyS,false));
    }
    return sval;
}

QString GPCReader::getVal(QString key, bool trimmed) {
    return getVal(icRawSection, key, trimmed);
}

QString GPCReader::getVal(QStringList source, QString key, bool trimmed) {
    QString sval;
    QRegularExpression re;
    QRegularExpressionMatch match;

    //qDebug() << " key: " << key;
    re.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);
    re.setPattern(key);
    qint32 line;
    line = source.indexOf(re);
    if (line > -1)
    {
        //qDebug() << "MATCH !!!!! at " << line;
        match = re.match(source[line]);
        //qDebug() << "Match: " << match;
        sval = match.captured(1);
        if (trimmed) sval = sval.trimmed();
        //qDebug() << "result: " << sval;
        return sval;
    }
    return sval;
}

QStringList GPCReader::getList(QString key, bool trimmed) {
    return getList(icRawSection, key, trimmed);
}

QStringList GPCReader::getList(QStringList source, QString key, bool trimmed) {
    QString sval;
    QStringList svallist;
    QRegularExpression re;
    QRegularExpressionMatch match;

    //qDebug() << " key: " << key;
    //re.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);
    re.setPattern(key);
    qint32 line = 0;

    while ((line = source.indexOf(re,line)) > -1)
    {
        match = re.match(source[line]);
        sval = match.captured(1);
        if (trimmed) sval = sval.trimmed();
        svallist.append(sval);
        //qDebug() << "result at " << line << " : " << sval;
        line++;
    }
    //qDebug() << "result: " << svallist;
    return svallist;
}
