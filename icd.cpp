#include "icd.h"
#include "ic.h"

ICD::ICD()
{

}

// qDebug() complete IC
void ICD::debug() {
    for(int idx = 0; idx < data.size(); idx++)
    {
        data[idx].debug();
    }
}

// qDebug() specific IC entry by lineNo
void ICD::debug(QString sName) {
    for(int idx = 0; idx < data.size(); idx++)
    {
        if( data[idx].lineNo.compare(sName)==0 )
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
