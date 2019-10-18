#ifndef PMEMMODEL_H
#define PMEMMODEL_H

#include <QAbstractTableModel>
#include "pmem.h"

class PMEMModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    QList<PMEM> pmemData;
    PMEMModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &) const override { return pmemData.count(); }
    int columnCount(const QModelIndex &) const override { return iRowCount; }
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void clear(void);

    bool exportCSV(QString filename);

private:
    int iRowCount=8;
};

#endif // PMEMMODEL_H
