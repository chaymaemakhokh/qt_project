// product.h
#pragma once

#include <QObject>
#include <QString>
#include "enums.h"
#include "produit_caracteristiques.h"
#include "produit_cycledevie.h"

/**
 * @brief Produit stocké dans un conteneur / palette.
 */
class Product : public QObject
{
    Q_OBJECT
public:
    explicit Product(QObject *parent = nullptr);

    // Identité
    QString idProduit() const;
    void setIdProduit(const QString &id);

    QString nom() const;
    void setNom(const QString &nom);

    TypeProduit type() const;
    void setType(TypeProduit type);

    double capaciteMax() const;
    void setCapaciteMax(double c);

    // Caractéristiques / cycle de vie (façades vers les sous-objets)
    ProduitAvecCaracteristiques& caracteristiques();
    const ProduitAvecCaracteristiques& caracteristiques() const;

    ProduitAvecCycleDeVie& cycleDeVie();
    const ProduitAvecCycleDeVie& cycleDeVie() const;

    // Raccourcis utiles
    double poids() const;
    double volume() const;
    QDate dateEntreeStock() const;
    QDate datePeremption() const;
    EtatProduit etat() const;

    bool estPerime(const QDate &aujourdHui = QDate::currentDate()) const;
    int joursAvantPeremption(const QDate &aujourdHui = QDate::currentDate()) const;

signals:
    void productChanged();

private:
    QString m_idProduit;
    QString m_nom;
    TypeProduit m_type {TypeProduit::Autre};
    double m_capaciteMax {0.0};

    ProduitAvecCaracteristiques m_caracteristiques;
    ProduitAvecCycleDeVie m_cycleDeVie;
};
