#pragma once
#include <QAbstractItemModel>
#include "warehouseitem.h"
#include "../domain/warehouse.h"

class WarehouseModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit WarehouseModel(Entrepot *entrepot, QObject *parent = nullptr);
    ~WarehouseModel();

    QModelIndex index(int row, int col, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    void refresh();

private:
    WarehouseItem *m_root;
    Entrepot *m_entrepot;

    void buildTree();
};
