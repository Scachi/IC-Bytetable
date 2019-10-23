#ifndef ICPROXY_H
#define ICPROXY_H

#include <QSortFilterProxyModel>

class ICProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ICProxy(QObject *parent = nullptr);
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void colSortAdd(int column, int order);
    void colSortNow();
    void setEditable( int index, bool FLAG);

private:
    int colSort[2][2];
    QMap<int,Qt::ItemFlags> flagMap;
    void setFlag(const int, Qt::ItemFlags);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

};

#endif // ICPROXY_H
