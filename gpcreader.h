#ifndef GPCREADER_H
#define GPCREADER_H

#include <QFileDialog>
#include <QStringList>
#include <QDebug>

class GPCReader
{
private:
    QString     gpcFilePath;        // current file processed
    QStringList includeList;        // list of found #include lines
    QStringList includeListDone;    // list of #include lines/files processed

    QStringList icFileList;         // file that contained the current parsed keywords
    QStringList icRawList;          // list containing current gpcFilePath complete files content
    QStringList icLineNo;           // line no in files where the keyword was found
    QStringList icNameList;         // keywords found
    QStringList icShortdescList;    // multiline shortdesc will have the line breaks replaced by some special char
    QStringList icByteOffsetList;   // ... and so on ...
    QStringList icByteOffsetHexList;
    QStringList icBitSizeList;
    QStringList icBitOffsetList;
    QStringList icDefaultValueList;
    QStringList icDefaultValueHexList;
    QStringList icNewValueList;
    QStringList icNewValueHexList;
    QStringList icQontrolTypeList;
    QStringList icItemList;         // multiple items will be merged into a single entry by adding some specialchar as a delimiter
    QStringList icMinValList;
    QStringList icMaxValList;
    QStringList icStepList;
    QStringList icDecimalsList;
    QStringList icVariableTypeList; // for future usage - creation of ICs: vartype=
    QStringList icVariableNameList; // for future usage - creation of ICs: varname=


public:
    GPCReader(QString s) {
        gpcFilePath = s;
        if (s=="clipboard") readClipboard();
        else readFile();
        parseRawList();
    }

    QStringList getICFileList()             {   return icFileList;              }
    QStringList getIncludeList()            {   return includeList;             }
    QStringList getIncludeListDone()        {   return includeListDone;         }
    QStringList getICLineNo()               {   return icLineNo;                }
    QStringList getICNameList()             {   return icNameList;              }
    QStringList getICShortdescList()        {   return icShortdescList;         }
    QStringList getICByteOffsetList()       {   return icByteOffsetList;        }
    QStringList getICByteOffsetHexList()    {   return icByteOffsetHexList;     }
    QStringList getICBitSizeList()          {   return icBitSizeList;           }
    QStringList getICBitOffsetList()        {   return icBitOffsetList;         }
    QStringList getICDefaultValueList()     {   return icDefaultValueList;      }
    QStringList getICDefaultValueHexList()  {   return icDefaultValueHexList;   }
    QStringList getICNewValueList()         {   return icNewValueList;          }
    QStringList getICNewValueHexList()      {   return icNewValueHexList;       }
    QStringList getICQontrolTypeList()      {   return icQontrolTypeList;       }
    QStringList getICItemList()             {   return icItemList;              }
    QStringList getICMinValList()           {   return icMinValList;            }
    QStringList getICMaxValList()           {   return icMaxValList;            }
    QStringList getICStepList()             {   return icStepList;              }
    QStringList getICDecimalsList()         {   return icDecimalsList;          }
    QStringList getICVariableNameList()     {   return icVariableNameList;      }
    QStringList getICVariableTypeList()     {   return icVariableTypeList;      }


private:
    void readFile();
    void readClipboard();
    void parseRawList();

};

#endif // GPCREADER_H
