#ifndef ICV_H
#define ICV_H

#include <QAbstractTableModel>
#include <QVector>
#include "ic.h"

class ICV
{
public:
    ICV();

    void debug();
    void debug(QString sName);
    void debug(qint32 iLineNo);

    QVector<IC> vector;

};

class ICVModel : public QAbstractTableModel {
   QVector<IC> m_data;
public:
   ICVModel(QObject * parent = {}) : QAbstractTableModel{parent} {}
   int rowCount(const QModelIndex &) const override { return m_data.count(); }
   int columnCount(const QModelIndex &) const override { return 3; }
   QVariant data(const QModelIndex &index, int role) const override {
      if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
      const auto & IC = m_data[index.row()];
      switch (index.column()) {
      case 0: return "123";//IC.make();
      case 1: return "456";//IC.model();
      case 2: return "789";//IC.registrationNumber();
      default: return {};
      };
   }
   QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
      if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
      switch (section) {
      case 0: return "Filename";
      case 1: return "Line";
      case 2: return "IC Name";
      default: return {};
      }
   }
   void append(const IC & ic) { // ?? IC ic ??
      beginInsertRows({}, m_data.count(), m_data.count());
      m_data.append(ic); // ?? IC ic ??
      endInsertRows();
   }
};

#endif // ICV_H

/* source: https://stackoverflow.com/questions/18964377/using-qtableview-with-a-model
class VehicleModel : public QAbstractTableModel {
   QList<Vehicle> m_data;
public:
   VehicleModel(QObject * parent = {}) : QAbstractTableModel{parent} {}
   int rowCount(const QModelIndex &) const override { return m_data.count(); }
   int columnCount(const QModelIndex &) const override { return 3; }
   QVariant data(const QModelIndex &index, int role) const override {
      if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
      const auto & vehicle = m_data[index.row()];
      switch (index.column()) {
      case 0: return vehicle.make();
      case 1: return vehicle.model();
      case 2: return vehicle.registrationNumber();
      default: return {};
      };
   }
   QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
      if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
      switch (section) {
      case 0: return "Make";
      case 1: return "Model";
      case 2: return "Reg.#";
      default: return {};
      }
   }
   void append(const Vehicle & vehicle) {
      beginInsertRows({}, m_data.count(), m_data.count());
      m_data.append(vehicle);
      endInsertRows();
   }
};
*/
