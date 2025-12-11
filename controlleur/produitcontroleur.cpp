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

QVector<std::shared_ptr<Product>>
ProduitControleur::rechercherParIdPartiel(const QString &idPartiel) const
{
    QVector<std::shared_ptr<Product>> res;
    const QString pattern = idPartiel.trimmed();

    for (const auto &p : m_produits) {
        if (!p) continue;
        if (pattern.isEmpty() ||
            p->idProduit().contains(pattern, Qt::CaseInsensitive)) {
            res.push_back(p);
        }
    }
    return res;
}

QVector<std::shared_ptr<Product>>
ProduitControleur::rechercherParFiltre(const FiltreProduit &f) const
{
    QVector<std::shared_ptr<Product>> res;
    const QString idPattern  = f.idPartiel.trimmed();
    const QString nomPattern = f.nomPartiel.trimmed();

    for (const auto &p : m_produits) {
        if (!p) continue;

        bool ok = true;

        // Id
        if (!idPattern.isEmpty() &&
            !p->idProduit().contains(idPattern, Qt::CaseInsensitive)) {
            ok = false;
        }

        // Type
        if (ok && f.filtrerParType && p->type() != f.type) {
            ok = false;
        }

        // Nom
        if (ok && !nomPattern.isEmpty() &&
            !p->nom().contains(nomPattern, Qt::CaseInsensitive)) {
            ok = false;
        }

        // Date d'entrée
        if (ok && f.filtrerParDateEntree &&
            p->dateEntreeStock() != f.dateEntree) {
            ok = false;
        }

        // Capacité max
        if (ok && f.filtrerParCapacite &&
            !qFuzzyCompare(p->capaciteMax() + 1.0, f.capaciteMax + 1.0)) {
            ok = false;
        }

        // Poids
        if (ok && f.filtrerParPoids &&
            !qFuzzyCompare(p->poids() + 1.0, f.poids + 1.0)) {
            ok = false;
        }

        // Volume
        if (ok && f.filtrerParVolume &&
            !qFuzzyCompare(p->volume() + 1.0, f.volume + 1.0)) {
            ok = false;
        }

        if (ok)
            res.push_back(p);
    }

    return res;
}

bool ProduitControleur::supprimerProduitParId(const QString &id)
{
    for (int i = 0; i < m_produits.size(); ++i) {
        const auto &p = m_produits[i];
        if (p && p->idProduit() == id) {
            m_produits.removeAt(i);
            return true;
        }
    }
    return false;
}

void ProduitControleur::vider()
{
    m_produits.clear();
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
