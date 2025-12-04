#pragma once
#include "product.h"

/**
 * @brief Produit périssable : possède poids, volume,
 * date d’entrée en stock et date de péremption.
 */
class ProduitAvecCycleDeVie : public Product
{
    Q_OBJECT
public:
    explicit ProduitAvecCycleDeVie(QObject *parent = nullptr);


    // ----- MÉTHODES POLYMORPHES -----
    double poids() const override { return m_poids; }
    double volume() const override { return m_volume; }

    QDate dateEntreeStock() const override { return m_entree; }
    QDate datePeremption() const override { return m_peremption; }

    EtatProduit etat() const override { return m_etat; }

    // ----- SETTERS -----
    void setPoids(double p) {
        if (m_poids == p) return;
        m_poids = p;
        emit productChanged();
    }

    void setVolume(double v) {
        if (m_volume == v) return;
        m_volume = v;
        emit productChanged();
    }

    void setDateEntreeStock(const QDate &d) {
        if (m_entree == d) return;
        m_entree = d;
        emit productChanged();
    }

    void setDatePeremption(const QDate &d) {
        if (m_peremption == d) return;
        m_peremption = d;
        emit productChanged();
    }

    void setEtat(EtatProduit e) {
        if (m_etat == e) return;
        m_etat = e;
        emit productChanged();
    }

private:
    double m_poids {0.0};
    double m_volume {0.0};

    QDate m_entree;        // pour FIFO
    QDate m_peremption;    // pour FEFO
    EtatProduit m_etat {EtatProduit::Stocke};
};
