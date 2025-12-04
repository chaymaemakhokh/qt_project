// container.h
#pragma once

#include <QObject>
#include <QList>
#include "enums.h"

class Product;

/**
 * @brief Conteneur physique de produits dans l'entrepôt.
 */
class Conteneur : public QObject
{
    Q_OBJECT
public:
    explicit Conteneur(QObject *parent = nullptr);

    QString idConteneur() const;
    void setIdConteneur(const QString &id);

    TypeConteneur type() const;
    void setType(TypeConteneur t);

    double capaciteMax() const;
    void setCapaciteMax(double c);

    const QList<Product*>& produits() const;

    bool peutAjouter(Product *p) const;
    bool ajouterProduit(Product *p);
    void retirerProduit(Product *p);

    double poidsTotal() const;

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
