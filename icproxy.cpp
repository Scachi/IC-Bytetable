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
