#include "warehousemodel.h"
#include "../domain/container.h"
#include "../domain/palette.h"
#include "../domain/product.h"

WarehouseModel::WarehouseModel(Entrepot *entrepot, QObject *parent)
    : QAbstractItemModel(parent),
    m_entrepot(entrepot)
{
    m_root = new WarehouseItem(NodeType::Entrepot, entrepot);
    buildTree();
}

WarehouseModel::~WarehouseModel()
{
    delete m_root;
}

void WarehouseModel::refresh()
{
    beginResetModel();
    delete m_root;
    m_root = new WarehouseItem(NodeType::Entrepot, m_entrepot);
    buildTree();
    endResetModel();
}

void WarehouseModel::buildTree()
{
    // --- GROUPE CONTENEURS ---
    WarehouseItem *groupCont = new WarehouseItem(NodeType::Conteneur, nullptr, m_root);
    m_root->appendChild(groupCont);

    for (Conteneur *c : m_entrepot->conteneurs()) {
        WarehouseItem *itemCont = new WarehouseItem(NodeType::Conteneur, c, groupCont);
        groupCont->appendChild(itemCont);

        for (Product *p : c->produits()) {
            WarehouseItem *itemProd = new WarehouseItem(NodeType::Produit, p, itemCont);
            itemCont->appendChild(itemProd);
        }
    }

    // --- GROUPE PALETTES ---
    WarehouseItem *groupPal = new WarehouseItem(NodeType::Palette, nullptr, m_root);
    m_root->appendChild(groupPal);

    for (Palette *p : m_entrepot->palettes()) {
        WarehouseItem *itemPal = new WarehouseItem(NodeType::Palette, p, groupPal);
        groupPal->appendChild(itemPal);

        for (const ElementsPalette &el : p->elements()) {
            for (Product *prod : el.produits()) {
                WarehouseItem *itemProd = new WarehouseItem(NodeType::Produit, prod, itemPal);
                itemPal->appendChild(itemProd);
            }
        }
    }
}


QModelIndex WarehouseModel::index(int row, int col, const QModelIndex &parentIndex) const
{
    if (!hasIndex(row, col, parentIndex))
        return QModelIndex();

    WarehouseItem *parentItem =
        parentIndex.isValid()
            ? static_cast<WarehouseItem*>(parentIndex.internalPointer())
            : m_root;

    WarehouseItem *child = parentItem->child(row);
    return child ? createIndex(row, col, child) : QModelIndex();
}

QModelIndex WarehouseModel::parent(const QModelIndex &childIndex) const
{
    if (!childIndex.isValid())
        return QModelIndex();

    WarehouseItem *child = static_cast<WarehouseItem*>(childIndex.internalPointer());
    WarehouseItem *parent = child->parent();

    if (!parent || parent == m_root)
        return QModelIndex();

    return createIndex(parent->row(), 0, parent);
}

int WarehouseModel::rowCount(const QModelIndex &parentIndex) const
{
    WarehouseItem *parent =
        parentIndex.isValid()
            ? static_cast<WarehouseItem*>(parentIndex.internalPointer())
            : m_root;

    return parent->childCount();
}

int WarehouseModel::columnCount(const QModelIndex &) const
{
    return 1; // une seule colonne : nom / identifiant
}

QVariant WarehouseModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    WarehouseItem *item = static_cast<WarehouseItem*>(index.internalPointer());

    // Groupe Conteneurs / Palettes → dataPtr() == nullptr
    if (item->dataPtr() == nullptr) {
        if (item->type() == NodeType::Conteneur)
            return "Conteneurs";
        if (item->type() == NodeType::Palette)
            return "Palettes";
    }

    switch (item->type()) {
    case NodeType::Entrepot:
        return "Entrepôt";

    case NodeType::Conteneur: {
        auto *c = static_cast<Conteneur*>(item->dataPtr());
        return c ? c->idConteneur() : "Conteneurs";
    }

    case NodeType::Palette: {
        auto *p = static_cast<Palette*>(item->dataPtr());
        return p ? QString("Palette %1").arg(p->idPalette()) : "Palettes";
    }

    case NodeType::Produit: {
        auto *prod = static_cast<Product*>(item->dataPtr());
        return prod ? prod->nom() : "Produit";
    }
    }

    return QVariant();
}

