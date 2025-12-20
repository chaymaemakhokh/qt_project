#include "container.h"
#include "product.h"
#include <QtGlobal>

Conteneur::Conteneur(QObject *parent)
    : QObject(parent)
{
}

QString Conteneur::idConteneur() const { return m_idConteneur; }

void Conteneur::setIdConteneur(const QString &id) {
    if (m_idConteneur == id) return;
    m_idConteneur = id;
    emit conteneurChanged();
}

TypeConteneur Conteneur::type() const { return m_type; }

void Conteneur::setType(TypeConteneur t) {
    if (m_type == t) return;
    m_type = t;
    emit conteneurChanged();
}

double Conteneur::capaciteMax() const { return m_capaciteMax; }

void Conteneur::setCapaciteMax(double c) {
    if (qFuzzyCompare(m_capaciteMax + 1.0, c + 1.0)) return;
    m_capaciteMax = c;
    emit conteneurChanged();
}

const QList<Product*>& Conteneur::produits() const { return m_produits; }


double Conteneur::poidsTotal() const {
    double total = 0.0;
    for (Product *p : m_produits)
        if (p) total += p->poids();
    return total;
}

double Conteneur::capaciteUtilisee() const {
    double used = 0.0;
    for (Product *p : m_produits) {
        if (p) used += p->capaciteMax();
    }
    return used;
}

double Conteneur::capaciteRestante() const {
    return m_capaciteMax - capaciteUtilisee();
}

bool Conteneur::peutAjouter(Product *p) const {
    if (!p) return false;
    const double capProduit = p->capaciteMax();
    if (capProduit < 0.0) return false;
    return (capaciteUtilisee() + capProduit) <= m_capaciteMax;
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

bool Conteneur::retirerProduitParId(const QString &idProduit)
{
    bool removed = false;

    auto it = m_produits.begin();
    while (it != m_produits.end()) {
        Product *p = *it;
        if (p && p->idProduit() == idProduit) {
            it = m_produits.erase(it);
            removed = true;
            emit produitRetire(p);
        } else {
            ++it;
        }
    }

    if (removed)
        emit conteneurChanged();

    return removed;
}

bool Conteneur::contientProduitId(const QString &idProduit) const
{
    const QString needle = idProduit.trimmed();
    if (needle.isEmpty()) return false;

    for (Product *p : m_produits) {
        if (p && p->idProduit() == needle)
            return true;
    }
    return false;
}

double Conteneur::capaciteUtiliseeSansProduit(const QString &idProduit) const
{
    const QString needle = idProduit.trimmed();
    double used = 0.0;

    for (Product *p : m_produits) {
        if (!p) continue;
        if (!needle.isEmpty() && p->idProduit() == needle) continue; // on ignore ce produit
        used += p->capaciteMax();
    }
    return used;
}
