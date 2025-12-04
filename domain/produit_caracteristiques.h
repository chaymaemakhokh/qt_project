#pragma once
#include "product.h"

/**
 * @brief Produit non périssable : possède poids, volume,
 * conditions de conservation et date d'entrée en stock.
 */
class ProduitAvecCaracteristiques : public Product
{
    Q_OBJECT
public:
    explicit ProduitAvecCaracteristiques(QObject *parent = nullptr);

    // ----- MÉTHODES POLYMORPHES -----
    double poids() const override { return m_poids; }
    double volume() const override { return m_volume; }

    // Produit non périssable → date entrée en stock utilisée pour FIFO
    QDate dateEntreeStock() const override { return m_dateEntreeStock; }

    // Produit non périssable → pas de date de péremption
    QDate datePeremption() const override { return {}; }

    EtatProduit etat() const override { return m_etat; }

    // ----- SETTERS SPÉCIFIQUES -----
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

    void setConditionsConservation(const QString &c) {
        if (m_conditions == c) return;
        m_conditions = c;
        emit productChanged();
    }

    void setDateEntreeStock(const QDate &d) {
        if (m_dateEntreeStock == d) return;
        m_dateEntreeStock = d;
        emit productChanged();
    }

    void setEtat(EtatProduit e) {
        if (m_etat == e) return;
        m_etat = e;
        emit productChanged();
    }

    // ----- GETTERS SPÉCIFIQUES -----
    QString conditionsConservation() const { return m_conditions; }

private:
    double m_poids {0.0};
    double m_volume {0.0};
    QString m_conditions;
    QDate m_dateEntreeStock;     // IMPORTANT pour FIFO
    EtatProduit m_etat {EtatProduit::Stocke};
};
