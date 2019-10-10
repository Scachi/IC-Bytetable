#include <QFont>
#include <QColor>

#include "pmemmodel.h"

PMEMModel::PMEMModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant PMEMModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) return {};

    if (orientation == Qt::Horizontal || orientation == Qt::Vertical) {
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

    if (role == Qt::BackgroundColorRole)
    {
        if (pmem.getBits()==0) return QColor("#02a1db");
        else
        {
            if (!pmem.isByte()) {
                if (pmem.getByBit(index.column())==0 &&
                    pmem.getByBitSize(index.column())==0) return QColor("#daed4b");
            }
        }
    }

    // font color
    if (role == Qt::ForegroundRole)
    {
        if (!pmem.isValid()) return QColor("#c70202");
    }

    // values normal tableview
    if (role == Qt::DisplayRole) {
        /*
        switch (index.column())
        {    // toInt for correct number ordering
            default: return {};
         }
         */
        if (pmem.getByByte()) return "X";
        else if (pmem.getByByteSize()) return "x";
        else {
            if (pmem.getBit(index.column())) {
                if (pmem.getByBit(index.column())) return "B";
                if (pmem.getByBitSize(index.column())) return "b";
            }
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
