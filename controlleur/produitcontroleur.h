#pragma once

#include <QObject>
#include <QVector>
#include <memory>

#include "./domain/product.h"
#include "./domain/produit_caracteristiques.h"
#include "./domain/produit_cycledevie.h"
#include "./domain/enums.h"

class ProduitControleur : public QObject
{
    Q_OBJECT
public:
    explicit ProduitControleur(QObject *parent = nullptr);

    std::shared_ptr<Product> ajouterProduitAvecCaracteristiques(
        const QString &id,
        const QString &nom,
        TypeProduit type,
        double capaciteMax,
        double poids,
        double volume,
        const QString &conditionsConservation,
        const QDate &dateEntree,
        EtatProduit etat);

    std::shared_ptr<Product> ajouterProduitAvecCycleDeVie(
        const QString &id,
        const QString &nom,
        TypeProduit type,
        double capaciteMax,
        double poids,
        double volume,
        const QDate &dateEntree,
        const QDate &datePeremption,
        EtatProduit etat);

    void debugPrintProduits() const;

private:
    QVector<std::shared_ptr<Product>> m_produits;
};
