// compatibility.cpp
#include "compatibility.h"

ContrainteCompatibilite::ContrainteCompatibilite(TypeProduit a, TypeProduit b, bool compatible)
    : m_typeA(a), m_typeB(b), m_compatible(compatible)
{
}

TypeProduit ContrainteCompatibilite::typeA() const { return m_typeA; }
void ContrainteCompatibilite::setTypeA(TypeProduit t) { m_typeA = t; }

TypeProduit ContrainteCompatibilite::typeB() const { return m_typeB; }
void ContrainteCompatibilite::setTypeB(TypeProduit t) { m_typeB = t; }

bool ContrainteCompatibilite::compatible() const { return m_compatible; }
void ContrainteCompatibilite::setCompatible(bool c) { m_compatible = c; }

bool ContrainteCompatibilite::concerne(TypeProduit a, TypeProduit b) const
{
    // compatible quelle que soit l'ordre a/b
    return (a == m_typeA && b == m_typeB) || (a == m_typeB && b == m_typeA);
}

// ---- ReglesCompatibilite ----

ReglesCompatibilite::ReglesCompatibilite(QObject *parent)
    : QObject(parent)
{
}

void ReglesCompatibilite::ajouterContrainte(const ContrainteCompatibilite &c)
{
    m_contraintes.append(c);
    emit rulesChanged();
}

bool ReglesCompatibilite::areCompatible(TypeProduit a, TypeProduit b) const
{
    for (const ContrainteCompatibilite &c : m_contraintes) {
        if (c.concerne(a, b))
            return c.compatible();
    }
    // Si aucune règle explicite : on considère compatible
    return true;
}

const QList<ContrainteCompatibilite> &ReglesCompatibilite::contraintes() const
{
    return m_contraintes;
}
