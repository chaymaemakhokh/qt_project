#pragma once

#include <QList>

class Product;

/**
 * @brief Élément d'une palette : associe un ou plusieurs Product* avec une quantité.
 *
 * Dans le diagramme UML, ElementsPalette représente :
 *   - une quantité
 *   - une liste de produits polymorphiques
 *
 * Avec la nouvelle architecture Product abstraite, m_produits contient des Product*
 * dérivés (ProduitAvecCaracteristiques, ProduitAvecCycleDeVie, etc.)
 */
class ElementsPalette
{
public:
    ElementsPalette() = default;

    // Quantité totale de cet élément
    int quantite() const { return m_quantite; }
    void setQuantite(int q) { m_quantite = q; }

    // Liste des produits polymorphiques
    const QList<Product*>& produits() const { return m_produits; }

    // Ajout d'un produit
    void ajouterProduit(Product *p)
    {
        if (p && !m_produits.contains(p))
            m_produits.append(p);
    }

    // Suppression d'un produit
    void retirerProduit(Product *p)
    {
        m_produits.removeAll(p);
    }

    // Poids total du lot (quantite × somme des poids des produits)
    double poidsTotal() const;

private:
    int m_quantite {1};
    QList<Product*> m_produits;
};
