#include "icproxy.h"
#include "mainwindow.h"

ICProxy::ICProxy(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

void ICProxy::colSortAdd(int column, int order) {
    // sortings first by bits and then by bytes
    if (colSort[1][0] == 6 && colSort[0][0] == 3)
    {
        if (colSort[1][0] == column) colSort[1][1] = order;
        else if (colSort[0][0] == column) colSort[0][1] = order;
        if (column == 3 || column == 6) return;
    }

    if (colSort[0][0] == 6 && column ==3)
    {
        colSort[1][0] = colSort[0][0];
        colSort[1][1] = colSort[0][1];
        colSort[0][0] = column;
        colSort[0][1] = order;
        return;
    }

    colSort[1][0] = -1;
    colSort[0][0] = column;
    colSort[0][1] = order;
}

void ICProxy::colSortNow() {
    if (colSort[1][0] > -1) sort(colSort[1][0], Qt::SortOrder(colSort[1][1]));
    if (colSort[0][0] > -1) sort(colSort[0][0], Qt::SortOrder(colSort[0][1]));
}

Qt::ItemFlags ICProxy::flags(const QModelIndex &index) const {
 // if the column alignment was set-up for all columns
 //if(flagMap.isEmpty()) return flagMap.value(Qt::NoItemFlags);
 // searches if the column alignment was set-up and returns the flag
 if(flagMap.contains(index.column())) return flagMap.value(index.column());
 return (Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
 //qDebug() << "flags: " << QSortFilterProxyModel::flags(index);
 // ItemIsSelectable|ItemIsEditable|ItemIsUserCheckable|ItemIsEnabled|ItemNeverHasChildren
 //return QSortFilterProxyModel::flags(index);
}

void ICProxy::setEditable(int index, bool FLAG) {
 if(FLAG)
 setFlag(index, Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled);
 else
 setFlag(index, Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
}

void ICProxy::setFlag(const int index, Qt::ItemFlags flag) {
    // checks if setFlagToAll() was called and, if so, rests flagMap
    // if(flagMap.contains(Qt::ItemIsEditable)) flagMap.clear();
    // inserts flag
    //qDebug() << "Flagmap:" << flagMap;
    flagMap.insert(index, flag);
    //qDebug() << "Flagmap:" << flagMap;
}


bool ICProxy::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    static int sortLast = -1;
    static int orderLast = -1;

    if (left.column() != sortLast || (this->sortOrder() != orderLast))
    {
        //qDebug() << "sort old: " << sortLast << " , now: " << left.column();
        sortLast=left.column();
        const_cast <ICProxy*> (this)->colSortAdd(left.column(),this->sortOrder());
    }


    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);

    if (leftData.type() == QVariant::Int) {
        qint32 leftInt = leftData.toInt();
        qint32 rightInt = rightData.toInt();
        return leftInt < rightInt;
    } else {
        QString leftString = leftData.toString();
        QString rightString = rightData.toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    }
}
