#include "icv.h"
#include "ic.h"

ICV::ICV()
{

}

// qDebug() complete IC
void ICV::debug() {
    for(int idx = 0; idx < vector.size(); idx++)
    {
        vector[idx].debug();
    }
}

// qDebug() specific IC entry by lineNo
void ICV::debug(QString sName) {
    for(int idx = 0; idx < vector.size(); idx++)
    {
        if( vector[idx].lineNo.compare(sName)==0 )
            vector[idx].debug();
    }
}

// qDebug() specific IC entry by lineNo
void ICV::debug(qint32 iLineNo) {
    for(int idx = 0; idx < vector.size(); idx++)
    {
        if(vector[idx].lineNo.toInt() == iLineNo)
            vector[idx].debug();
    }
}
