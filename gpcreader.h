#ifndef GPCREADER_H
#define GPCREADER_H

#include <QFileDialog>
#include <QStringList>
#include <QDebug>

#include "ic.h"
#include "icd.h"
#include "pmem.h"
#include "pmemd.h"

class GPCReader
{
public:
    GPCReader(QString sDir, QString sFilePath);

public:
    ICD *icData;
    PMEMD *pmemData;

    bool getGPCICFound();
    bool isValid();

private:
    QString     gpcSelectedDir;     // the path part of the file that was initially opened
    QString     gpcSelectedFilePath;// selected filename from filedialog or recent file list
    bool        gpcICFound;         // true when an IC definition has been found
    bool        gpcValid;           // errors found ?

    QString     gpcCurrentFilePath; // current file processed
    QStringList gpcPathList;        // pathes to search files in
    qint32      gpcICBegin;         // line containing <cfgdesc>
    qint32      gpcICEnd;           // line containing </cfgdesc>
    QStringList includeList;        // list of found #include lines
    QStringList includeListDone;    // list of #include lines/files processed

    QStringList gpcRawList;         // current GPCFilePath complete files content

    QStringList icRawList;          // current GPCFilePath InteractiveConfiguration filecontent

    QStringList icRawSection;       // current [somename] raw section

    QString     icFileName;         // file that contained the current parsed keywords

    QString     icLineNo;           // line no in file where the keyword was found
    QString     icName;             // keywords found
    QString     icControl;

private:
    void    parse();
    void    readFile(QString path="");
    void    readClipboard();

    QString locateFile(QString path);
    void    addPath(QFile file);
    void    addPath(QString path);


    void parseGPCRawList();
    void findHeaderFiles(QStringList source);

    bool gpcRawHasIC();
    void parseICRawList();
    void parseICSection(qint32 line);

    QStringList getShortDesc();
    QString     getVal(QString key, bool trimmed=true);
    QString     getVal(QStringList source, QString key, bool trimmed=true);
    QStringList getList(QString key, bool trimmed=true);
    QStringList getList(QStringList source, QString key, bool trimmed=true);

};

#endif // GPCREADER_H
