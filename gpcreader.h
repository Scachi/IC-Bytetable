#ifndef GPCREADER_H
#define GPCREADER_H

#include <QFileDialog>
#include <QStringList>
#include <QDebug>

#include "ic.h"

class GPCReader
{
private:
    QString     GPCSelectedDir;     // the path part of the file that was initially opened
    QString     GPCSelectedFilePath;// selected filename from filedialog or recent file list
    bool        GPCICFound;         // true when an IC definition has been found

    QString     GPCCurrentFilePath; // current file processed
    QStringList GPCPathList;        // pathes to search files in
    qint32      GPCICBegin;         // line containing <cfgdesc>
    qint32      GPCICEnd;           // line containing </cfgdesc>
    QStringList IncludeList;        // list of found #include lines
    QStringList IncludeListDone;    // list of #include lines/files processed

    QStringList GPCRawList;         // current GPCFilePath complete files content

    QStringList ICRawList;          // current GPCFilePath InteractiveConfiguration filecontent

    QStringList ICRawSection;       // current [somename] raw section

    QString     ICFileName;         // file that contained the current parsed keywords

    QString     ICLineNo;           // line no in files where the keyword was found
    QString     ICName;             // keywords found
    QString     ICControl;
    QStringList ICItem;             // multiple items will be merged into a single entry by adding some special char as a delimiter
    QString     ICMinVal;
    QString     ICMaxVal;
    QString     ICStep;
    QString     ICDecimals;

    QString     ICCollapsible;
    QString     ICGroup;
    QString     ICGroupCol;
    QString     ICColor;

    QString     ICVarType;          // for future usage - creation of ICs: vartype=
    QString     ICVarName;          // for future usage - creation of ICs: varname=
    QString     ICComment;          // for future usage - creation of ICs: comment=

    QStringList ICShortdesc;        // multiline shortdesc will have the line breaks replaced by some special char
    QString     ICByteOffset;       // ... and so on ...
    QString     ICByteOffsetHex;
    QString     ICBitSize;
    QString     ICBitOffset;
    QString     ICDefaultVal;
    QString     ICDefaultValHex;
    QString     ICNewVal;
    QString     ICNewValHex;

    QVector<IC> ICVec;

public:
    GPCReader(QString sDir, QString sFilePath) {
        GPCSelectedDir = sDir;
        GPCSelectedFilePath = sFilePath;
        GPCCurrentFilePath = "";
        //qDebug() << " Sdir / sfilepath: " << sDir << " / " << sFilePath;
        parse();
    }
/*
    bool        getICFound()                {   return GPCICFound;              }
    QStringList getIncludeList()            {   return IncludeList;             }
    QStringList getIncludeListDone()        {   return IncludeListDone;         }
    QStringList getICFileList()             {   return ICFileList;              }
    QStringList getICLineNo()               {   return ICLineNo;                }
    QStringList getICCommentList()          {   return ICCommentList;           }
    QStringList getICNameList()             {   return ICNameList;              }
    QStringList getICShortdescList()        {   return ICShortdescList;         }
    QStringList getICByteOffsetList()       {   return ICByteOffsetList;        }
    QStringList getICByteOffsetHexList()    {   return ICByteOffsetHexList;     }
    QStringList getICBitSizeList()          {   return ICBitSizeList;           }
    QStringList getICBitOffsetList()        {   return ICBitOffsetList;         }
    QStringList getICDefaultValueList()     {   return ICDefaultValueList;      }
    QStringList getICDefaultValueHexList()  {   return ICDefaultValueHexList;   }
    QStringList getICNewValueList()         {   return ICNewValueList;          }
    QStringList getICNewValueHexList()      {   return ICNewValueHexList;       }
    QStringList getICQontrolTypeList()      {   return ICQontrolTypeList;       }
    QStringList getICItemList()             {   return ICItemList;              }
    QStringList getICMinValList()           {   return ICMinValList;            }
    QStringList getICMaxValList()           {   return ICMaxValList;            }
    QStringList getICStepList()             {   return ICStepList;              }
    QStringList getICDecimalsList()         {   return ICDecimalsList;          }
    QStringList getICVariableNameList()     {   return ICVariableNameList;      }
    QStringList getICVariableTypeList()     {   return ICVariableTypeList;      }
*/

private:
    void parse();
    void readFile(QString path="");
    void readClipboard();

    QString locateFile(QString path);
    void addPath(QFile file);
    void addPath(QString path);


    void parseGPCRawList();
    void findHeaderFiles(QStringList source);

    bool gpcRawHasIC();
    void parseICRawList();
    void parseICSection(qint32 line);

    QStringList GetShortdesc();
    QString GetVal(QString key, bool trimmed=true);
    QString GetVal(QStringList source, QString key, bool trimmed=true);
    QStringList GetList(QString key, bool trimmed=true);
    QStringList GetList(QStringList source, QString key, bool trimmed=true);

};

#endif // GPCREADER_H
