#include <QFont>
#include <QColor>

#include "pmemmodel.h"

PMEMModel::PMEMModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant PMEMModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
    if (role != Qt::DisplayRole) return {};

    if (orientation == Qt::Horizontal) {
        switch (section)
        {
            default: return section;
        }
    }
    else if (orientation == Qt::Vertical) {
        return section;
    }
    return {};
}

QVariant PMEMModel::data(const QModelIndex &index, int role) const
{
    const auto & pmem = pmemData[index.row()]; // access to data

    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignTop;
    }


    /*
    // font style
    if (role == Qt::FontRole && index.column() == 0)
    { // First column items are bold.
            QFont font;
            font.setBold(true);
            return font;
    }
    */

    // font color
    if (role == Qt::ForegroundRole)
    {
        switch(index.column())
        {
            case  7: if (pmem.byByte) return QColor("#02a1db");
                     if (pmem.bySize) return QColor("#f79a05");
                     if (pmem.byBit) return QColor("#c70202");
                     return QColor("green");
            default: return {};
        }
    }

    // values normal tableview
    if (role == Qt::DisplayRole) {
        switch (index.column())
        {    // toInt for correct number ordering
            default: return {};
         }
    }

    return QVariant();
}

Qt::ItemFlags PMEMModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsUserCheckable | Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

void PMEMModel::clear(void) {
    beginResetModel();
    pmemData.clear();
    endResetModel();
}
