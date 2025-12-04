
#pragma once

#include <QList>

class Product;

/**
 * @brief Élément d'une palette : produit + quantité.
 */
class ElementsPalette
{
public:
    ElementsPalette() = default;

    int quantite() const { return m_quantite; }
    void setQuantite(int q) { m_quantite = q; }

    const QList<Product*>& produits() const { return m_produits; }
    void ajouterProduit(Product *p) { if (p && !m_produits.contains(p)) m_produits.append(p); }
    void retirerProduit(Product *p) { m_produits.removeAll(p); }

private:
    int m_quantite {1};
    QList<Product*> m_produits;
};
