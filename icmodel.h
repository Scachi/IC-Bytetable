#ifndef ICMODEL_H
#define ICMODEL_H

#include <QAbstractTableModel>
#include "ic.h"
#include "icd.h"

class ICModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ICD  *icData = nullptr;
    ICModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &) const override { return icData->getDataCount(); }
    int columnCount(const QModelIndex &) const override { return icData->getFieldCount(); }
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void clear(void);

    bool importConfigString(QString cfgstring);
    QString createConfigString(bool all=false) const;
    QString createConfigStringBits() const;

private:


signals:
    void editCompleted(const QString &);

};

#endif // ICMODEL_H
