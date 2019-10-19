#include <QFile>
#include <QFont>
#include <QColor>
#include <QItemSelectionModel>
#include "icmodel.h"
#include "mainwindow.h"
#include "xtra.h"

ICModel::ICModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    columnNames << "Filename" << "Line" << "Name";
    columnNames << "Byte\nOffset" << "Byte\nOffhex" << "Bit\nSize" << "Bit\nOffset";
    columnNames << "Status" << "Info";
    columnNames << "Def\nVal" << "Def\nHex" << "New\nVal" << "New\nHex";
    columnNames << "Control" << "Items" << "Min\nVal" << "Max\nVal" << "Decimals" << "Step";
    columnNames << "ShortDesc" << "Collapsible" << "Group" << "GroupCol" << "Color" << "Border";
}

QVariant ICModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
    if (role != Qt::DisplayRole) return {};



    if (orientation == Qt::Horizontal) {
        if (section < columnNames.size()) return columnNames.at(section);
        else return {};
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
            case  2: if (ic.group.length() > 0 || ic.groupCol.length() >0)
                        return {};
                     return QColor(ic.getColor()); // color the [Name] of ungrouped controls
            case  7: if (ic.valid == 2) return QColor("#02a1db"); // info
                     if (ic.valid == 4) return QColor("#f79a05"); // warn
                     if (ic.valid == 8) return QColor("#c70202"); // error
                     return QColor("green"); // ok
            //case 19: return QColor(Qt::gray);
            case  23: return QColor(ic.getColor()); // color for [Name]
            default: return {};
        }
    }

    // values formatted for tooltip
    if (role == Qt::ToolTipRole) {
        switch (index.column())
        {
            case  0: return ic.getFileNameFull();
            case  2: return ic.getNameToolTip();
            case  8: return ic.getInfoToolTip();
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
/*
            case 25: return ic.getVarType();
            case 26: return ic.getVarName();
            case 27: return ic.getComment();
*/
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
        //QModelIndexList indexes = MainWindow().getSelectionTableView();
        //qDebug() << "selected: " << indexes;

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

bool ICModel::exportCSV(QString filename)
{
    QString csvQuote="\"";
    QString csvDelimiter=",";
    QString fname = filename.replace("_data.txt",".txt").replace("_chart.txt",".txt").replace(".txt","_data.txt");
    fname = fname.replace("_data.csv",".csv").replace("_chart.csv",".csv").replace(".csv","_data.csv");
    QFile file(fname);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&file);
    // column titles
    out << csvQuote << "Index" << csvQuote << csvDelimiter;
    for (int col=0; col < this->iRowCount ; col++)
    {
        if (col>0) out << csvDelimiter;
        QString title;
        if (col < this->columnNames.size())
            title = this->columnNames.at(col);
        else title = QString::number(col);
        out << csvQuote << title.replace("\n"," ") << csvQuote;
    }
    out << "\n";
    // content
    for (int row=0; row<this->icData.count(); row++)
    {
        if (row>0) out << "\n";
        out << csvQuote << row << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getFileName() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getLineNo() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getName() << csvQuote << csvDelimiter;

        out << csvQuote << icData[row].getByteOffset() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getByteOffsetHex() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getBitSize() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getBitOffset() << csvQuote << csvDelimiter;

        out << csvQuote << icData[row].getValid() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getInfo().replace("\n"," ") << csvQuote << csvDelimiter;

        out << csvQuote << icData[row].getDefaultVal() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getDefaultValHex() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getNewVal() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getNewValHex() << csvQuote << csvDelimiter;

        out << csvQuote << icData[row].getControl() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getItemCount() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getMinVal() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getMaxVal() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getDecimals() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getStep() << csvQuote << csvDelimiter;

        out << csvQuote << icData[row].getShortDescPlain(99).replace("\n"," ") << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getCollapsible() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getGroup() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getGroupCol() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getColor() << csvQuote << csvDelimiter;
        out << csvQuote << icData[row].getBorder() << csvQuote;
    }
    return true;
}

bool ICModel::importConfigString(QString cfgstring)
{
    //qDebug() << "Import";
    if (cfgstring.startsWith("GIVICFG:"))
        cfgstring = cfgstring.mid(8);
    //qDebug() << "cfgstring: " << cfgstring;

    //parse the string - all values are hex
    int hexIndex;
    int byteOffset = 0;
    QString hexValue;
    int bitSize;

    for (hexIndex = 0; hexIndex < cfgstring.length() - 1; hexIndex++)
    {
        hexValue = cfgstring.mid(hexIndex, 1);
        // check if current character is a '#' then read nn (the byteoffset)
        if (hexValue.compare("#") ==0 ) // byteoffset found
        {
            bool ok;
            hexIndex += 1;
            hexValue = cfgstring.mid(hexIndex, 2);
            byteOffset = hexValue.toInt(&ok,16); // update byteoffset value
            //qDebug() << "ok:" << ok << "HexIndex: " << hexIndex << "Jumping to Byteoffset: " << byteOffset;
            hexIndex += 1; // increase to next hex to read/check
        }
        else if (hexValue.compare(":") == 0 ) // normal value definition
        {
            hexIndex += 1;
            bitSize = getBitsizeFromByteoffset(byteOffset);
            if (bitSize < 8) bitSize = 8;
            hexValue = cfgstring.mid(hexIndex, bitSize / 4);
            //qDebug() << "ImportValues byteoffset:'" << byteOffset << "' | hexvalue'" << hexValue; //4debug
            setByteoffset2Hex(byteOffset,hexValue);
            //qDebug() << "byteOffset: " << byteOffset << " (size:"<< bitSize <<") | Parsing string pos " << hexIndex << " - hex: " << hexValue;
            hexIndex += (bitSize / 4) - 1;  // update hex index position
            byteOffset += (bitSize / 8);    // update byteoffet position
        }
        else if (hexValue.compare("|") == 0 ) // bitwise OR (set a bit)
        {
            bool ok;
            hexIndex += 1;
            bitSize = getBitsizeFromByteoffset(byteOffset);
            if (bitSize < 8) bitSize = 8;
            hexValue = cfgstring.mid(hexIndex, bitSize / 4);
            //qDebug() << "ImportValues byteoffset:'" << byteOffset << "' | hexvalue'" << hexValue; //4debug
            QString binOR = QString("%1").arg(hexValue.toULongLong(&ok, 16), 8, 2, QChar('0'));
            QString hexSrc = getValHexFromByteoffset(&ok, byteOffset);
            //QString binSrc = QString("%1").arg(hexSrc.toULongLong(&ok, 16), 8, 2, QChar('0'));
            QString binSrc = XTRA::xHex2Bin(hexSrc,QString::number(bitSize));
            qDebug() << "BINARY OR: ";
            qDebug() << binSrc << " <- src mask";
            qDebug() << binOR << " <- mod mask";
            //setByteoffset2Hex(byteOffset,hexValue);
            //qDebug() << "byteOffset: " << byteOffset << " (size:"<< bitSize <<") | Parsing string pos " << hexIndex << " - hex: " << hexValue;
            hexIndex += (bitSize / 4) - 1;  // update hex index position
            byteOffset += (bitSize / 8);    // update byteoffet position
        }
        else if (hexValue.compare("&") == 0 ) // bitwise AND (clear a bit)
        {
            bool ok;
            hexIndex += 1;
            bitSize = getBitsizeFromByteoffset(byteOffset); // get bitsize
            if (bitSize < 8) bitSize = 8;
            hexValue = cfgstring.mid(hexIndex, bitSize / 4); // read the hexvalue
            //qDebug() << "ImportValues byteoffset:'" << byteOffset << "' | hexvalue'" << hexValue; //4debug
            QString binAND = QString("%1").arg(hexValue.toULongLong(&ok, 16), 8, 2, QChar('0'));
            QString hexSrc = getValHexFromByteoffset(&ok, byteOffset);
            //QString binSrc = QString("%1").arg(hexSrc.toULongLong(&ok, 16), 8, 2, QChar('0'));
            QString binSrc = XTRA::xHex2Bin(hexSrc,QString::number(bitSize));
            qDebug() << "BINARY AND: ";
            qDebug() << binSrc << " <- src mask";
            qDebug() << binAND << " <- mod mask";
            //setByteoffset2Hex(byteOffset,hexValue);
            //qDebug() << "byteOffset: " << byteOffset << " (size:"<< bitSize <<") | Parsing string pos " << hexIndex << " - hex: " << hexValue;
            hexIndex += (bitSize / 4) - 1;  // update hex index position
            byteOffset += (bitSize / 8);    // update byteoffet position
        } else {
            //hexIndex += 1;
            bitSize = getBitsizeFromByteoffset(byteOffset); // get bitsize
            if (bitSize < 8) bitSize = 8;
            hexValue = cfgstring.mid(hexIndex, bitSize / 4); // read the hexvalue
            //qDebug() << "ImportValues byteoffset:'" << byteOffset << "' | hexvalue'" << hexValue; //4debug
            setByteoffset2Hex(byteOffset,hexValue); // import the hexvalue
            //qDebug() << "byteOffset: " << byteOffset << " (size:"<< bitSize <<") | Parsing string pos " << hexIndex << " - hex: " << hexValue;
            hexIndex += (bitSize / 4) - 1; // update hex index position
            byteOffset += (bitSize / 8); // update byteoffet position
        }
    }
    endResetModel();
    return true;
}

int ICModel::getBitsizeFromByteoffset(int byteoffset)
{
    for (int row=0; row<this->icData.count(); row++)
    {
        if (icData[row].byteOffset.length() > 0 && icData[row].byteOffset.toInt()==byteoffset)
            return icData[row].bitSize.toInt();
    }
    return 8;
}

QString ICModel::getValHexFromByteoffset(bool *ok,int byteoffset)
{
    for (int row=0; row<this->icData.count(); row++)
    {
        if (icData[row].byteOffset.length() > 0 && icData[row].byteOffset.toInt()==byteoffset) {
            *ok=true;
            if (icData[row].newValHex.length()>0) return icData[row].newValHex;
            else return icData[row].defaultValHex;
        }
    }
    *ok=false;
    return "00";
}

bool ICModel::setByteoffset2Hex(int byteoffset, QString hexvalue, bool bitsonly)
{
    bool found=false;
    for (int row=0; row<this->icData.count(); row++)
    {
        if (icData[row].byteOffset.length() > 0  && icData[row].byteOffset.toInt()==byteoffset)
        {
            if (bitsonly &&  icData[row].bitSize.toInt()>=8) continue;
            //qDebug() << " Setting " << icData[row].byteOffset << " to " << hexvalue;
            icData[row].newValHex = hexvalue;
            found = true;
        }
    }
    return found;
}

QString ICModel::exportConfigString() const
{
    QString exportString="none";
    return exportString;
}
