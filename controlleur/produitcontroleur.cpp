#include "produitcontroleur.h"
#include <QDebug>

ProduitControleur::ProduitControleur(QObject *parent)
    : QObject(parent)
{
}

std::shared_ptr<Product> ProduitControleur::ajouterProduitAvecCaracteristiques(
    const QString &id,
    const QString &nom,
    TypeProduit type,
    double capaciteMax,
    double poids,
    double volume,
    const QString &conditionsConservation,
    const QDate &dateEntree,
    EtatProduit etat)
{
    auto p = std::make_shared<ProduitAvecCaracteristiques>();

    p->setIdProduit(id);
    p->setNom(nom);
    p->setType(type);
    p->setCapaciteMax(capaciteMax);
    p->setPoids(poids);
    p->setVolume(volume);
    p->setConditionsConservation(conditionsConservation);
    p->setDateEntreeStock(dateEntree);
    p->setEtat(etat);

    m_produits.push_back(p);
    return p;
}

std::shared_ptr<Product> ProduitControleur::ajouterProduitAvecCycleDeVie(
    const QString &id,
    const QString &nom,
    TypeProduit type,
    double capaciteMax,
    double poids,
    double volume,
    const QDate &dateEntree,
    const QDate &datePeremption,
    EtatProduit etat)
{
    auto p = std::make_shared<ProduitAvecCycleDeVie>();

    p->setIdProduit(id);
    p->setNom(nom);
    p->setType(type);
    p->setCapaciteMax(capaciteMax);
    p->setPoids(poids);
    p->setVolume(volume);
    p->setDateEntreeStock(dateEntree);
    p->setDatePeremption(datePeremption);
    p->setEtat(etat);

    m_produits.push_back(p);
    return p;
}

void ProduitControleur::debugPrintProduits() const
{
    qDebug().noquote() << "===== LISTE DES PRODUITS =====";
    for (const auto &p : m_produits) {
        if (!p) continue;
        QString ligne = QStringLiteral(
                            "ID=%1 | Nom=%2 | Type=%3 | CapMax=%4 | Poids=%5 | Volume=%6 | Etat=%7")
                            .arg(p->idProduit())
                            .arg(p->nom())
                            .arg(toString(p->type()))
                            .arg(p->capaciteMax())
                            .arg(p->poids())
                            .arg(p->volume())
                            .arg(toString(p->etat()));
        qDebug().noquote() << ligne;
    }
    qDebug().noquote() << "===============================";
}
Product* ProduitControleur::getProduitParId(const QString &id) const
{
    for (const auto &p : m_produits)
    {
        if (p && p->idProduit() == id)
            return p.get();
    }
    return nullptr;
}
