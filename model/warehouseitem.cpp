#include "warehouseitem.h"

WarehouseItem::WarehouseItem(NodeType type, QObject *dataPtr, WarehouseItem *parent)
    : m_type(type), m_ptr(dataPtr), m_parent(parent)
{
}

WarehouseItem::~WarehouseItem()
{
    qDeleteAll(m_children);
}

void WarehouseItem::appendChild(WarehouseItem *child)
{
    m_children.append(child);
}

WarehouseItem *WarehouseItem::child(int row) const
{
    return m_children.value(row);
}

int WarehouseItem::childCount() const
{
    return m_children.count();
}

int WarehouseItem::row() const
{
    if (!m_parent)
        return 0;
    return m_parent->m_children.indexOf(const_cast<WarehouseItem*>(this));
}

WarehouseItem *WarehouseItem::parent() const
{
    return m_parent;
}

NodeType WarehouseItem::type() const
{
    return m_type;
}

QObject *WarehouseItem::dataPtr() const
{
    return m_ptr;
}
