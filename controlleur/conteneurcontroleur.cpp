#include "conteneurcontroleur.h"
#include <QDebug>

ConteneurControleur::ConteneurControleur(QObject *parent)
    : QObject(parent)
{
}

void ConteneurControleur::ajouterConteneur(const QString &id,
                                           TypeConteneur type,
                                           double capaciteMax)
{
    auto c = std::make_shared<Conteneur>();
    c->setIdConteneur(id);
    c->setType(type);
    c->setCapaciteMax(capaciteMax);

    m_conteneurs.push_back(c);
}

Conteneur* ConteneurControleur::trouverConteneurParId(const QString &id) const
{
    for (const auto &c : m_conteneurs) {
        if (c && c->idConteneur() == id)
            return c.get();
    }
    return nullptr;
}

bool ConteneurControleur::ajouterProduitAuConteneur(
    const QString &idConteneur,
    const std::shared_ptr<Product> &produit)
{
    if (!produit) return false;

    Conteneur *c = trouverConteneurParId(idConteneur);
    if (!c) return false;

    return c->ajouterProduit(produit.get());
}

QVector<std::shared_ptr<Conteneur>>
ConteneurControleur::rechercherParIdPartiel(const QString &idPartiel) const
{
    QVector<std::shared_ptr<Conteneur>> res;
    const QString pattern = idPartiel.trimmed();

    for (const auto &c : m_conteneurs) {
        if (!c) continue;
        if (pattern.isEmpty() ||
            c->idConteneur().contains(pattern, Qt::CaseInsensitive)) {
            res.push_back(c);
        }
    }
    return res;
}

QVector<std::shared_ptr<Conteneur>>
ConteneurControleur::rechercherParFiltre(const FiltreConteneur &f) const
{
    QVector<std::shared_ptr<Conteneur>> res;
    const QString pattern = f.idPartiel.trimmed();

    for (const auto &c : m_conteneurs) {
        if (!c) continue;

        bool ok = true;

        // Id partiel
        if (!pattern.isEmpty() &&
            !c->idConteneur().contains(pattern, Qt::CaseInsensitive)) {
            ok = false;
        }

        // Type
        if (ok && f.filtrerParType && c->type() != f.type) {
            ok = false;
        }

        // Capacité max (égalité stricte pour l'instant)
        if (ok && f.filtrerParCapacite &&
            !qFuzzyCompare(c->capaciteMax() + 1.0, f.capaciteMax + 1.0)) {
            ok = false;
        }

        if (ok)
            res.push_back(c);
    }

    return res;
}

bool ConteneurControleur::supprimerConteneurParId(const QString &id)
{
    for (int i = 0; i < m_conteneurs.size(); ++i) {
        const auto &c = m_conteneurs[i];
        if (c && c->idConteneur() == id) {
            m_conteneurs.removeAt(i);
            return true;
        }
    }
    return false;
}

bool ConteneurControleur::supprimerProduitDansTousLesConteneursParId(
    const QString &idProduit)
{
    bool removed = false;
    for (const auto &c : m_conteneurs) {
        if (!c) continue;
        if (c->retirerProduitParId(idProduit))
            removed = true;
    }
    return removed;
}

void ConteneurControleur::debugPrintConteneurs() const
{
    qDebug().noquote() << "===== LISTE DES CONTENEURS =====";

    for (const auto &c : m_conteneurs) {
        if (!c) continue;

        QString ligne = QStringLiteral(
                            "ID=%1 | Type=%2 | CapMax=%3 | NbProduits=%4 | PoidsTotal=%5")
                            .arg(c->idConteneur())
                            .arg(toString(c->type()))
                            .arg(c->capaciteMax())
                            .arg(c->produits().size())
                            .arg(c->poidsTotal());

        qDebug().noquote() << ligne;
    }

    qDebug().noquote() << "================================";
}
