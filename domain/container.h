#pragma once
#include <QObject>
#include <QList>
#include "enums.h"

class Product;

/**
 * @brief Représente un conteneur dans l'entrepôt.
 * Stocke des Product* polymorphiques.
 */
class Conteneur : public QObject
{
    Q_OBJECT

public:
    explicit Conteneur(QObject *parent = nullptr);
    ~Conteneur() override = default;

    // ----- Identité -----
    QString idConteneur() const;
    void setIdConteneur(const QString &id);

    // ----- Type -----
    TypeConteneur type() const;
    void setType(TypeConteneur t);

    // ----- Capacité -----
    double capaciteMax() const;
    void setCapaciteMax(double c);

    // ----- Produits -----
    const QList<Product*>& produits() const;

    double poidsTotal() const;
    bool peutAjouter(Product *p) const;
    bool ajouterProduit(Product *p);
    void retirerProduit(Product *p);

    bool retirerProduitParId(const QString &idProduit);

signals:
    void conteneurChanged();
    void produitAjoute(Product *p);
    void produitRetire(Product *p);

private:
    QString m_idConteneur;
    TypeConteneur m_type {TypeConteneur::Normal};
    double m_capaciteMax {0.0};
    QList<Product*> m_produits;
};
