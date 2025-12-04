// produit_caracteristiques.h
#pragma once

#include "enums.h"
#include <QString>

/**
 * @brief Caractéristiques physiques d'un produit.
 */
class ProduitAvecCaracteristiques
{
public:
    ProduitAvecCaracteristiques() = default;

    double poids() const { return m_poids; }
    void setPoids(double p) { m_poids = p; }

    double volume() const { return m_volume; }
    void setVolume(double v) { m_volume = v; }

    QString conditionsConservation() const { return m_conditionsConservation; }
    void setConditionsConservation(const QString &c) { m_conditionsConservation = c; }

    EtatProduit etat() const { return m_etat; }
    void setEtat(EtatProduit e) { m_etat = e; }

private:
    double m_poids {0.0};
    double m_volume {0.0};
    QString m_conditionsConservation;
    EtatProduit m_etat {EtatProduit::Stocke};   // doublon avec cycle de vie, mais présent dans le diagramme UML
};
