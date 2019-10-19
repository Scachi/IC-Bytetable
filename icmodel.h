#ifndef ICMODEL_H
#define ICMODEL_H

#include <QAbstractTableModel>
#include "ic.h"
#include "icd.h"

class ICModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    QList<IC> icData;
    QStringList columnNames;
    ICModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &) const override { return icData.count(); }
    int columnCount(const QModelIndex &) const override { return iRowCount; }
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void clear(void);

    bool exportCSV(QString filename);
    bool importConfigString(QString cfgstring);
    int getBitsizeFromByteoffset(int byteoffset);
    QString getValHexFromByteoffset(bool *ok, int byteoffset);
    bool setByteoffset2Hex(int byteoffset, QString hexvalue,bool bitsonly=false);
    QString exportConfigString() const;

private:
    int iRowCount = 25;

signals:
    void editCompleted(const QString &);

};

#endif // ICMODEL_H
