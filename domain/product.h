#pragma once
#include <QObject>
#include <QString>
#include <QDate>
#include "enums.h"

/**
 * @brief Classe abstraite Product (classe mère de tous les produits).
 *
 * Contient uniquement :
 *  - Identité du produit
 *  - Type
 *  - Capacité maximale
 *
 * Le reste (poids, volume, dates, état…) est défini dans les sous‑classes :
 *   - ProduitAvecCaracteristiques
 *   - ProduitAvecCycleDeVie
 */
class Product : public QObject
{
    Q_OBJECT
public:
    explicit Product(QObject *parent = nullptr);
    virtual ~Product() = default;

    // ---- IDENTITÉ ----
    QString idProduit() const { return m_idProduit; }
    void setIdProduit(const QString &id) {
        if (m_idProduit == id) return;
        m_idProduit = id;
        emit productChanged();
    }

    QString nom() const { return m_nom; }
    void setNom(const QString &n) {
        if (m_nom == n) return;
        m_nom = n;
        emit productChanged();
    }

    TypeProduit type() const { return m_type; }
    void setType(TypeProduit t) {
        if (m_type == t) return;
        m_type = t;
        emit productChanged();
    }

    double capaciteMax() const { return m_capaciteMax; }
    void setCapaciteMax(double c) {
        if (qFuzzyCompare(m_capaciteMax, c)) return;
        m_capaciteMax = c;
        emit productChanged();
    }

    // ---- MÉTHODES POLYMORPHES ----
    virtual double poids() const = 0;
    virtual double volume() const = 0;

    // Date d'entrée en stock → nécessaire pour FIFO (même produit non périssable)
    virtual QDate dateEntreeStock() const = 0;

    // Périssable uniquement : retourne {} pour les produits non périssables
    virtual QDate datePeremption() const = 0;

    // État : stocké / expédié (ou autre)
    virtual EtatProduit etat() const = 0;

    // Méthode utilitaire : test de péremption
    bool estPerime(const QDate &jour = QDate::currentDate()) const {
        return datePeremption().isValid() && datePeremption() < jour;
    }

signals:
    void productChanged();

protected:
    QString m_idProduit;
    QString m_nom;
    TypeProduit m_type {TypeProduit::Autre};
    double m_capaciteMax {0.0};
};
