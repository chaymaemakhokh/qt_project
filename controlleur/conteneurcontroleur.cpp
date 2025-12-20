#include "conteneurcontroleur.h"
#include <QDebug>
#include <QtGlobal>

#include <QRandomGenerator>

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
ConteneurControleur::rechercherParFiltre(const FiltreConteneur &f) const
{
    QVector<std::shared_ptr<Conteneur>> res;

    const QString idNeedle = f.idPartiel.trimmed();

    for (const auto &c : m_conteneurs)
    {
        if (!c) continue;

        bool ok = true;

        // 1) ID partiel (contains)
        if (!idNeedle.isEmpty()) {
            // contains insensible à la casse (tu peux mettre CaseSensitive si tu veux)
            if (!c->idConteneur().contains(idNeedle, Qt::CaseInsensitive))
                ok = false;
        }

        // 2) Type exact
        if (ok && f.filtrerParType) {
            if (c->type() != f.type)
                ok = false;
        }

        // 3) Capacité exacte
        if (ok && f.filtrerParCapacite) {
            // exact "numérique" (tolérance) pour éviter les bugs de double
            if (!qFuzzyCompare(c->capaciteMax() + 1.0, f.capaciteMax + 1.0))
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

bool ConteneurControleur::peutAjouterProduitAuConteneur(const QString &idConteneur,
                                                        double capaciteProduit) const
{
    Conteneur *c = trouverConteneurParId(idConteneur);
    if (!c) return false;
    return capaciteProduit >= 0.0 && capaciteProduit <= c->capaciteRestante();
}

bool ConteneurControleur::idExiste(const QString &id) const
{
    const QString needle = id.trimmed();
    if (needle.isEmpty()) return false;

    for (const auto &c : m_conteneurs) {
        if (c && c->idConteneur().compare(needle, Qt::CaseInsensitive) == 0)
            return true;
    }
    return false;
}

QString ConteneurControleur::genererIdConteneurUnique(int longueur) const
{
    static const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    QString id;
    do {
        id = "C"; // ✅ commence par C
        for (int i = 0; i < longueur; ++i) {
            int idx = QRandomGenerator::global()->bounded(chars.size());
            id.append(chars[idx]);
        }
    } while (idExiste(id));

    return id;
}

bool ConteneurControleur::peutModifierCapaciteProduit(const QString &idProduit,
                                                      double nouvelleCapacite,
                                                      QString *conteneurBloquant,
                                                      double *capaciteRestante) const
{
    const QString pid = idProduit.trimmed();
    if (pid.isEmpty()) return true; // rien à vérifier
    if (nouvelleCapacite < 0.0) return false;

    for (const auto &csp : m_conteneurs) {
        if (!csp) continue;
        Conteneur *c = csp.get();
        if (!c->contientProduitId(pid)) continue;

        const double usedSans = c->capaciteUtiliseeSansProduit(pid);
        const double capMax = c->capaciteMax();
        const double restanteApresSans = capMax - usedSans;

        if (usedSans + nouvelleCapacite > capMax + 1e-9) {
            if (conteneurBloquant) *conteneurBloquant = c->idConteneur();
            if (capaciteRestante) *capaciteRestante = restanteApresSans;
            return false;
        }
    }
    return true;
}

void ConteneurControleur::vider()
{
    m_conteneurs.clear();
}

