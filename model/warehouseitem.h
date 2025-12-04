#pragma once

#include <QList>
#include <QVariant>

/**
 * @brief Type de noeud dans l'arbre.
 */
enum class NodeType {
    Entrepot,
    Conteneur,
    Produit,
    Palette
};

/**
 * @brief Un noeud de l'arbre du modèle hiérarchique.
 *
 * WarehouseItem NE CONTIENT PAS les données métier.
 * Il contient UN POINTEUR vers l'objet réel (Product, Conteneur,
 * Palette ou Entrepot).
 */
class WarehouseItem
{
public:
    WarehouseItem(NodeType type, QObject *dataPtr, WarehouseItem *parent = nullptr);
    ~WarehouseItem();

    WarehouseItem *child(int row) const;
    int childCount() const;
    int row() const;
    WarehouseItem *parent() const;

    void appendChild(WarehouseItem *child);

    NodeType type() const;
    QObject *dataPtr() const;

private:
    QList<WarehouseItem*> m_children;
    WarehouseItem *m_parent {nullptr};

    NodeType m_type;
    QObject *m_ptr;     // pointeur vers Product, Conteneur, Palette ou Entrepot
};
