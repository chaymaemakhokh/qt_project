// compatibility.h
#pragma once

#include <QObject>
#include <QList>
#include "enums.h"

/**
 * @brief Contrainte de compatibilité entre deux types de produits.
 */
class ContrainteCompatibilite
{
public:
    ContrainteCompatibilite(TypeProduit a = TypeProduit::Autre,
                            TypeProduit b = TypeProduit::Autre,
                            bool compatible = true);

    TypeProduit typeA() const;
    void setTypeA(TypeProduit t);

    TypeProduit typeB() const;
    void setTypeB(TypeProduit t);

    bool compatible() const;
    void setCompatible(bool c);

    bool concerne(TypeProduit a, TypeProduit b) const;

private:
    TypeProduit m_typeA;
    TypeProduit m_typeB;
    bool m_compatible {true};
};

/**
 * @brief Ensemble des règles de compatibilité.
 */
class ReglesCompatibilite : public QObject
{
    Q_OBJECT
public:
    explicit ReglesCompatibilite(QObject *parent = nullptr);

    void ajouterContrainte(const ContrainteCompatibilite &c);
    bool areCompatible(TypeProduit a, TypeProduit b) const;

    const QList<ContrainteCompatibilite>& contraintes() const;

signals:
    void rulesChanged();

private:
    QList<ContrainteCompatibilite> m_contraintes;
};
