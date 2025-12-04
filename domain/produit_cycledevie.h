// produit_cycledevie.h
#pragma once

#include "enums.h"
#include <QDate>

/**
 * @brief Informations de cycle de vie d'un produit.
 */
class ProduitAvecCycleDeVie
{
public:
    ProduitAvecCycleDeVie() = default;

    QDate dateEntreeStock() const { return m_dateEntreeStock; }
    void setDateEntreeStock(const QDate &d) { m_dateEntreeStock = d; }

    QDate datePeremption() const { return m_datePeremption; }
    void setDatePeremption(const QDate &d) { m_datePeremption = d; }

    EtatProduit etat() const { return m_etat; }
    void setEtat(EtatProduit e) { m_etat = e; }

private:
    QDate m_dateEntreeStock;
    QDate m_datePeremption;
    EtatProduit m_etat {EtatProduit::Stocke};
};
