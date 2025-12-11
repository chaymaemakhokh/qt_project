#pragma once

#include <QObject>
#include <QVector>
#include <memory>

#include "./domain/product.h"
#include "./domain/produit_caracteristiques.h"
#include "./domain/produit_cycledevie.h"
#include "./domain/enums.h"

struct FiltreProduit
{
    QString idPartiel;              // vide = pas de filtre
    bool   filtrerParType = false;
    TypeProduit type;

    QString nomPartiel;             // vide = pas de filtre

    bool   filtrerParDateEntree = false;
    QDate  dateEntree;

    bool   filtrerParCapacite = false;
    double capaciteMax = 0.0;

    bool   filtrerParPoids = false;
    double poids = 0.0;

    bool   filtrerParVolume = false;
    double volume = 0.0;
};

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

    QVector<std::shared_ptr<Product>>
    rechercherParIdPartiel(const QString &idPartiel) const;

    QVector<std::shared_ptr<Product>>
    rechercherParFiltre(const FiltreProduit &filtre) const;

    bool supprimerProduitParId(const QString &id);

    const QVector<std::shared_ptr<Product>>& produits() const { return m_produits; }

     void vider();

    void debugPrintProduits() const;

private:
    QVector<std::shared_ptr<Product>> m_produits;
};
