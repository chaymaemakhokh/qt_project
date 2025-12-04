// product.cpp
#include "product.h"

Product::Product(QObject *parent)
    : QObject(parent)
{
}

QString Product::idProduit() const { return m_idProduit; }
void Product::setIdProduit(const QString &id)
{
    if (m_idProduit == id) return;
    m_idProduit = id;
    emit productChanged();
}

QString Product::nom() const { return m_nom; }
void Product::setNom(const QString &nom)
{
    if (m_nom == nom) return;
    m_nom = nom;
    emit productChanged();
}

TypeProduit Product::type() const { return m_type; }
void Product::setType(TypeProduit type)
{
    if (m_type == type) return;
    m_type = type;
    emit productChanged();
}

double Product::capaciteMax() const { return m_capaciteMax; }
void Product::setCapaciteMax(double c)
{
    if (qFuzzyCompare(m_capaciteMax, c)) return;
    m_capaciteMax = c;
    emit productChanged();
}

ProduitAvecCaracteristiques &Product::caracteristiques() { return m_caracteristiques; }
const ProduitAvecCaracteristiques &Product::caracteristiques() const { return m_caracteristiques; }

ProduitAvecCycleDeVie &Product::cycleDeVie() { return m_cycleDeVie; }
const ProduitAvecCycleDeVie &Product::cycleDeVie() const { return m_cycleDeVie; }

double Product::poids() const { return m_caracteristiques.poids(); }
double Product::volume() const { return m_caracteristiques.volume(); }
QDate Product::dateEntreeStock() const { return m_cycleDeVie.dateEntreeStock(); }
QDate Product::datePeremption() const { return m_cycleDeVie.datePeremption(); }
EtatProduit Product::etat() const { return m_cycleDeVie.etat(); }

bool Product::estPerime(const QDate &aujourdHui) const
{
    if (!m_cycleDeVie.datePeremption().isValid()) return false;
    return m_cycleDeVie.datePeremption() < aujourdHui;
}

int Product::joursAvantPeremption(const QDate &aujourdHui) const
{
    if (!m_cycleDeVie.datePeremption().isValid()) return INT_MAX;
    return aujourdHui.daysTo(m_cycleDeVie.datePeremption());
}
