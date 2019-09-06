#ifndef ICPROXY_H
#define ICPROXY_H

#include <QSortFilterProxyModel>

class ICProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ICProxy(QObject *parent = nullptr);

    void colSortAdd(int column, int order);
    void colSortNow();

private:
    int colSort[2][2];

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

};

#endif // ICPROXY_H
