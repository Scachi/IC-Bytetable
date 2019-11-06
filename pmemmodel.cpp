#include <QFile>
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
    const auto & pmem = pmemData->data[index.row()]; // access to data

    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignTop;
    }



    // font style
    /*
    if (role == Qt::FontRole && index.column() == 0) // First column items.
    {
            QFont font;
            font.setBold(true);
            return font;
    }
    */


    if (role == Qt::BackgroundColorRole)
    {
        if (pmem.getBits()==0) return QColor("#32CD32"); // #02a1db
        else
        {
            if (!pmem.isByte()) {
                if (pmem.getByBit(index.column())==0 &&
                    pmem.getByBitSize(index.column())==0) return QColor("#FFFF00"); // #daed4b
            }
        }
    }

    // font color
    if (role == Qt::ForegroundRole)
    {
        if (pmem.isByte() && !pmem.isValid()) return QColor("#c70202");
        if (pmem.isBit() && !pmem.isValidBit(index.column())) return QColor("#c70202");
    }

    // values normal tableview
    if (role == Qt::DisplayRole) {
        return pmem.getTableMark(index.column());

    }

    return QVariant();
}

Qt::ItemFlags PMEMModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsUserCheckable | Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

void PMEMModel::clear(void) {
    beginResetModel();
    if (pmemData != nullptr) pmemData->data.clear();
    endResetModel();
}
