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
    for(int idx = 0; idx < data.size(); idx++)
    {
        if (namesProcessed.contains(data[idx].name)) continue;
        namesProcessed.append(data[idx].name);
        searchForNames(idx,data[idx].name,true,QString("name %1 not unique").arg(data[idx].name));
    }
    return true;
}

bool ICD::searchForNames(int srcid, QString name, bool mark, QString msg, qint8 severity) {
    bool found=false;
    for(int idx = 0; idx < data.size(); idx++)
    {
        if (idx==srcid) continue;
        if (data[idx].name.compare(name)==0)
        {
            found=true;
            if (mark) data[idx].msgAdd(msg,severity);
        }
    }
    if (found && mark) data[srcid].msgAdd(msg,severity);
    return found;
}
