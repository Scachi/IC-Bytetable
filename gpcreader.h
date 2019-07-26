#ifndef GPCREADER_H
#define GPCREADER_H

#include <QFileDialog>
#include <QStringList>
#include <QDebug>

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

    QStringList GPCRawList;         // list containing current GPCFilePath complete files content

    QStringList ICRawList;          // list containing current GPCFilePath ic filecontent
    QStringList ICCommentList;      // OWN for notes: single line only, line has to be directly above the [Name] and has to start with //
    QStringList ICFileList;         // file that contained the current parsed keywords
    QStringList ICLineNo;           // line no in files where the keyword was found
    QStringList ICNameList;         // keywords found
    QStringList ICShortdescList;    // multiline shortdesc will have the line breaks replaced by some special char
    QStringList ICByteOffsetList;   // ... and so on ...
    QStringList ICByteOffsetHexList;
    QStringList ICBitSizeList;
    QStringList ICBitOffsetList;
    QStringList ICDefaultValueList;
    QStringList ICDefaultValueHexList;
    QStringList ICNewValueList;
    QStringList ICNewValueHexList;
    QStringList ICQontrolTypeList;
    QStringList ICItemList;         // multiple items will be merged into a single entry by adding some special char as a delimiter
    QStringList ICMinValList;
    QStringList ICMaxValList;
    QStringList ICStepList;
    QStringList ICDecimalsList;
    QStringList ICVariableTypeList; // for future usage - creation of ICs: vartype=
    QStringList ICVariableNameList; // for future usage - creation of ICs: varname=


public:
    GPCReader(QString sDir, QString sFilePath) {
        GPCSelectedDir = sDir;
        GPCSelectedFilePath = sFilePath;
        GPCCurrentFilePath = "";
        //qDebug() << " Sdir / sfilepath: " << sDir << " / " << sFilePath;
        parse();
    }

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


private:
    QString locateFile(QString path);
    void addPath(QFile file);
    void addPath(QString path);
    void readFile(QString path="");
    void readClipboard();
    void parse();
    void parseGPCRawList();
    bool gpcRawHasIC();
    void parseICRawList();

    void findHeaderFiles(QStringList source);
    void findICNameLines();

};

#endif // GPCREADER_H
