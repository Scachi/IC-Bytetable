#include "icd.h"
#include "ic.h"

ICD::ICD()
{
    bitsUsed=0;
}

ICD::~ICD()
{

}

// qDebug() complete IC
void ICD::debug() {
    for(int idx = 0; idx < data.size(); idx++)
    {
        data[idx].debug();
    }
}

// qDebug() specific IC entry by Name
void ICD::debug(QString sName) {
    for(int idx = 0; idx < data.size(); idx++)
    {
        if( data[idx].name.compare(sName)==0 )
            data[idx].debug();
    }
}

// qDebug() specific IC entry by lineNo
void ICD::debug(qint32 iLineNo) {
    for(int idx = 0; idx < data.size(); idx++)
    {
        if(data[idx].lineNo.toInt() == iLineNo)
            data[idx].debug();
    }
}

// validate the names of all controls to be unique
bool ICD::validateUniqueNames() {
    QStringList namesProcessed;
    bool unique=true;
    for(int idx = 0; idx < data.size(); idx++)
    {
        if (namesProcessed.contains(data[idx].name)) continue;
        namesProcessed.append(data[idx].name);
        if (!searchForNames(idx,data[idx].name,true,QString("name %1 not unique").arg(data[idx].name)))
        {
            unique=false;
        }
    }
    return unique;
}

bool ICD::searchForNames(int srcid, QString name, bool mark, QString msg, qint8 severity) {
    bool unique=true;
    for(int idx = 0; idx < data.size(); idx++)
    {
        if (idx==srcid) continue;
        if (data[idx].name.compare(name)==0)
        {
            unique=false;
            if (mark) data[idx].msgAdd(msg,severity);
        }
    }
    if (!unique && mark) data[srcid].msgAdd(msg,severity);
    return unique;
}

bool ICD::isValid() {
    bool valid=true;
    for(int idx = 0; idx < data.size(); idx++)
    {
        if (data[idx].valid>2) valid=false;
    }
    return valid;
}
