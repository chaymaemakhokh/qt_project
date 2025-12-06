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
