#include <QFont>
#include <QColor>
#include "icmodel.h"
#include "mainwindow.h"

ICModel::ICModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant ICModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
    if (role != Qt::DisplayRole) return {};

    if (orientation == Qt::Horizontal) {
        switch (section)
        {
            case  0: return "Filename";
            case  1: return "Line";
            case  2: return "Name";

            case  3: return "Byte Offset";
            case  4: return "Byte Offhex";
            case  5: return "Bit Size";
            case  6: return "Bit Offset";

            case  7: return "Status";
            case  8: return "Info";

            case  9: return "Def Val";
            case 10: return "Def Hex";
            case 11: return "New Val";
            case 12: return "New Hex";

            case 13: return "Control";
            case 14: return "Items";
            case 15: return "Min Val";
            case 16: return "Max Val";
            case 17: return "Decimals";
            case 18: return "Step";

            case 19: return "ShortDesc";
            case 20: return "Callapsible";
            case 21: return "Group";
            case 22: return "GroupCol";
            case 23: return "Color";
            case 24: return "Border";

            case 25: return "VarType";
            case 26: return "VarName";
            case 27: return "Comment";

            default: return {};
        }
    }
    else if (orientation == Qt::Vertical) {
        return section+1;
    }

    return {};
}

QVariant ICModel::data(const QModelIndex &index, int role) const
{
    const auto & ic = icData[index.row()]; // access to data

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
            case  2: return QColor(ic.getColor()); // color the [Name]
            //case 19: return QColor(Qt::gray);
            case  23: return QColor(ic.getColor()); // color the [Name]
            default: return {};
        }
    }

    // values formatted for tooltip
    if (role == Qt::ToolTipRole) {
        switch (index.column())
        {
            case  0: return ic.getFileNameFull();
            case  2: return ic.getNameToolTip();
            case 14: return ic.getItemNamesToolTip();
            case 19: return ic.getShortDesc(40);
            case 20: return ic.getCollapsibleToolTip();
            default: return {};
        }
    }

    // checkboxes
    if (role == Qt::CheckStateRole) {
        switch (index.column())
        {
            case  0: if (ic.checked) return Qt::Checked;
                     else return Qt::Unchecked;

        }
    }

    // values normal tableview
    if (role == Qt::DisplayRole) {
        switch (index.column())
        {    // toInt for correct number ordering
            case  0: return ic.getFileName();
            case  1: return ic.getLineNo().toInt();
            case  2: return ic.getName();

            case  3: if (ic.getByteOffset().length() > 0) return ic.getByteOffset().toInt(); else return {};
            case  4: return ic.getByteOffsetHex();
            case  5: if (ic.getBitSize().length() > 0) return ic.getBitSize().toInt(); else return {};
            case  6: if (ic.getBitOffset().length() > 0) return ic.getBitOffset().toInt(); else return {};

            case  7: return ic.getValid();
            case  8: return ic.getInfo();

            case  9: return ic.getDefaultVal();
            case 10: return ic.getDefaultValHex();
            case 11: return ic.getNewVal();
            case 12: return ic.getNewValHex();

            case 13: return ic.getControl();
            case 14: return ic.getItemCount("#");
            case 15: return ic.getMinVal();
            case 16: return ic.getMaxVal();
            case 17: return ic.getDecimals();
            case 18: return ic.getStep();

            case 19: return ic.getShortDescPlain(100);
            case 20: return ic.getCollapsible();
            case 21: return ic.getGroup();
            case 22: return ic.getGroupCol();
            case 23: return ic.getColor();
            case 24: return ic.getBorder();

            case 25: return ic.getVarType();
            case 26: return ic.getVarName();
            case 27: return ic.getComment();

            default: return {};
         }
    }

    /*
    if (role == Qt::EditRole) {

    }
    */
    return QVariant();
}

Qt::ItemFlags ICModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsUserCheckable | Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

bool ICModel::setData(const QModelIndex &index,
                              const QVariant &value, int role)
{
    auto & ic = icData[index.row()]; // access to data

    if (role == Qt::EditRole) {
        /*
        if (!checkIndex(index)) return false;
        ic.checked = value.toBool();
        qDebug() << "checked: " << ic.checked;
        QString result="result";
        //emit dataChanged(index, index, {role});
        emit editCompleted(result);
        return true;
        */
    }
    if (role == Qt::CheckStateRole) {
        ic.checked = value.toBool();
        //qDebug() << "checked: " << ic.checked;
        return true;
    }
    return false;
}

void ICModel::clear(void) {
    beginResetModel();
    icData.clear();
    endResetModel();
}
