#include "palette.h"
#include "product.h"
#include <QtGlobal>    // pour qFuzzyCompare

Palette::Palette(QObject *parent)
    : QObject(parent)
{
}

QString Palette::idPalette() const
{
    return m_idPalette;
}

void Palette::setIdPalette(const QString &id)
{
    if (m_idPalette == id)
        return;
    m_idPalette = id;
    emit paletteChanged();
}

QString Palette::destination() const
{
    return m_destination;
}

void Palette::setDestination(const QString &dest)
{
    if (m_destination == dest)
        return;
    m_destination = dest;
    emit paletteChanged();
}

QDate Palette::dateEnvoiPrevue() const
{
    return m_dateEnvoiPrevue;
}

void Palette::setDateEnvoiPrevue(const QDate &d)
{
    if (m_dateEnvoiPrevue == d)
        return;
    m_dateEnvoiPrevue = d;
    emit paletteChanged();
}

double Palette::capaciteMax() const
{
    return m_capaciteMax;
}

void Palette::setCapaciteMax(double c)
{
    if (qFuzzyCompare(m_capaciteMax, c))
        return;
    m_capaciteMax = c;
    emit paletteChanged();
}

const QList<ElementsPalette> &Palette::elements() const
{
    return m_elements;
}

QList<ElementsPalette> &Palette::elementsRef()
{
    return m_elements;
}

double Palette::poidsTotal() const
{
    double total = 0.0;
    for (const ElementsPalette &e : m_elements) {
        total += e.poidsTotal();   // on réutilise la logique d'ElementsPalette
    }
    return total;
}

bool Palette::peutAjouter(Product *p, const ReglesCompatibilite *regles) const
{
    if (!p)
        return false;

    // 1) vérifier la capacité en poids
    if (p->poids() + poidsTotal() > m_capaciteMax)
        return false;

    // 2) compatibilité avec tous les produits déjà présents
    if (regles) {
        for (const ElementsPalette &e : m_elements) {
            for (Product *autre : e.produits()) {
                if (autre && !regles->areCompatible(p->type(), autre->type()))
                    return false;
            }
        }
    }

    return true;
}

bool Palette::ajouterProduit(Product *p, const ReglesCompatibilite *regles)
{
    if (!peutAjouter(p, regles))
        return false;

    // Si un ElementsPalette contient déjà ce produit, on augmente la quantité
    for (ElementsPalette &e : m_elements) {
        if (e.produits().contains(p)) {
            e.setQuantite(e.quantite() + 1);
            emit paletteChanged();
            return true;
        }
    }

    // Sinon on crée un nouvel "élément palette"
    ElementsPalette e;
    e.setQuantite(1);
    e.ajouterProduit(p);
    m_elements.append(e);

    emit paletteChanged();
    return true;
}
