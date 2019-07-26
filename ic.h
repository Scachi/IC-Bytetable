#ifndef IC_H
#define IC_H
#include <QStringList>
#include <QDebug>

class IC
{
public:
    IC();

private:
    QString     FileName;   // filename this section was found in
    QStringList RawData;    // complete raw data of the section

    qint32      Line;       // line number the [section] tag was found in
    QString     Name;       // name of the [section]
    QString     Control;
    QStringList Items;
    QString     MinVal;
    QString     MaxVal;
    QString     Step;
    QString     Decimals;

    QString     VariableDataType; // for future usage - creation of ICs: vartype=
    QString     VariableName;     // for future usage - creation of ICs: varname=

    QString     Comment;    // OWN for notes: single line only, line has to be directly above the [Name] and has to start with //
    QStringList Shortdesc;
    QString     ByteOffset;
    QString     ByteOffsetHex;
    QString     BitSize;
    QString     BitOffset;
    QString     DefaultVal;
    QString     DefaultValHex;
    QString     NewVal;
    QString     NewValHex;
};

#endif // IC_H
