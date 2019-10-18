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
    pmemData.clear();
    endResetModel();
}

bool PMEMModel::exportCSV(QString filename)
{
    QString csvQuote="\"";
    QString csvDelimiter=",";
    QString fname = filename.replace("_data.txt",".txt").replace("_chart.txt",".txt").replace(".txt","_chart.txt");
    fname = fname.replace("_data.csv",".csv").replace("_chart.csv",".csv").replace(".csv","_chart.csv");
    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&file);
    // column titles
    out << csvQuote << "Byte" << csvQuote << csvDelimiter;
    for (int col = 0; col < this->iRowCount ; col++)
    {
        if (col > 0) out << csvDelimiter;
        out << csvQuote << col << csvQuote;
    }
    out << csvDelimiter;
    out << csvQuote << "Status" << csvQuote;
    out << "\n";
    // content
    for (int row=0; row<this->pmemData.count(); row++)
    {
        if (row > 0 ) out << "\n";
        out << csvQuote << row << csvQuote << csvDelimiter;
        for (int col=0; col < this->iRowCount ; col++)
        {
            if (col > 0) out << csvDelimiter;
            PMEM pm = this->pmemData[row];
            out << csvQuote << pm.getTableMark(col) << csvQuote;
        }
        out << csvDelimiter;
        out << csvQuote;
        if (this->pmemData[row].isValid()) out << "OK";
        else out <<  "ERROR";
        out << csvQuote;
    }
    return true;
}
