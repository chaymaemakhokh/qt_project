#include "container.h"
#include "product.h"
#include <QtGlobal>

Conteneur::Conteneur(QObject *parent)
    : QObject(parent)
{
}

QString Conteneur::idConteneur() const {
    return m_idConteneur;
}

void Conteneur::setIdConteneur(const QString &id) {
    if (m_idConteneur == id) return;
    m_idConteneur = id;
    emit conteneurChanged();
}

TypeConteneur Conteneur::type() const {
    return m_type;
}

void Conteneur::setType(TypeConteneur t) {
    if (m_type == t) return;
    m_type = t;
    emit conteneurChanged();
}

double Conteneur::capaciteMax() const {
    return m_capaciteMax;
}

void Conteneur::setCapaciteMax(double c) {
    if (qFuzzyCompare(m_capaciteMax, c)) return;
    m_capaciteMax = c;
    emit conteneurChanged();
}

const QList<Product*>& Conteneur::produits() const {
    return m_produits;
}

double Conteneur::poidsTotal() const {
    double total = 0.0;
    for (Product *p : m_produits)
        if (p) total += p->poids();
    return total;
}

bool Conteneur::peutAjouter(Product *p) const {
    if (!p) return false;
    return p->poids() + poidsTotal() <= m_capaciteMax;
}

bool Conteneur::ajouterProduit(Product *p) {
    if (!peutAjouter(p)) return false;

    if (!m_produits.contains(p)) {
        m_produits.append(p);
        emit produitAjoute(p);
        emit conteneurChanged();
    }
    return true;
}

void Conteneur::retirerProduit(Product *p) {
    if (m_produits.removeAll(p) > 0) {
        emit produitRetire(p);
        emit conteneurChanged();
    }
}
