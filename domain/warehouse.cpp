#include "warehouse.h"
#include <algorithm>
#include <QtGlobal>
#include "produit_caracteristiques.h"
#include "produit_cycledevie.h"
#include "palette.h"
#include "container.h"

Entrepot::Entrepot(QObject *parent)
    : QObject(parent)
{
}

// ----------------------- infos entrepôt ----------------------------
QString Entrepot::idEntrepot() const { return m_idEntrepot; }

void Entrepot::setIdEntrepot(const QString &id)
{
    if (m_idEntrepot == id) return;
    m_idEntrepot = id;
    emit entrepotChanged();
}

QString Entrepot::nom() const { return m_nom; }

void Entrepot::setNom(const QString &n)
{
    if (m_nom == n) return;
    m_nom = n;
    emit entrepotChanged();
}

QString Entrepot::adresse() const { return m_adresse; }

void Entrepot::setAdresse(const QString &a)
{
    if (m_adresse == a) return;
    m_adresse = a;
    emit entrepotChanged();
}

double Entrepot::surface() const { return m_surface; }

void Entrepot::setSurface(double s)
{
    if (qFuzzyCompare(m_surface, s)) return;
    m_surface = s;
    emit entrepotChanged();
}

// ----------------------- accès collections ----------------------------
const QList<Conteneur*>& Entrepot::conteneurs() const { return m_conteneurs; }
const QList<Palette*>& Entrepot::palettes() const { return m_palettes; }

QList<Product*> Entrepot::tousLesProduits() const
{
    QList<Product*> res;
    for (Conteneur *c : m_conteneurs)
    {
        if (!c) continue;
        for (Product *p : c->produits())
        {
            if (p && !res.contains(p))
                res.append(p);
        }
    }
    return res;
}

// ----------------------- conteneurs ----------------------------
Conteneur* Entrepot::creerConteneur()
{
    auto *c = new Conteneur(this);
    m_conteneurs.append(c);
    emit entrepotChanged();
    return c;
}

void Entrepot::supprimerConteneur(Conteneur *c)
{
    if (!c) return;
    m_conteneurs.removeAll(c);
    c->deleteLater();
    emit entrepotChanged();
}

// ----------------------- produits ----------------------------
Product* Entrepot::creerProduitDans(Conteneur *c, TypeProduit type)
{
    if (!c) return nullptr;

    Product *p = nullptr;

    // 🎯 Décision : QUELS types sont périssables ?
    bool estPerissable =
        (type == TypeProduit::Alimentaire ||
         type == TypeProduit::Medicament);

    if (estPerissable)
        p = new ProduitAvecCycleDeVie(c);        // périssable → cycle de vie
    else
        p = new ProduitAvecCaracteristiques(c);  // non périssable → caractéristiques

    p->setType(type);  // IMPORTANT → garder la cohérence de l’objet

    if (c->ajouterProduit(p))
        emit entrepotChanged();

    return p;
}

void Entrepot::supprimerProduitDe(Conteneur *c, Product *p)
{
    if (!c || !p) return;
    c->retirerProduit(p);
    p->deleteLater();
    emit entrepotChanged();
}

// ----------------------- palettes ----------------------------
Palette* Entrepot::creerPalette()
{
    auto *p = new Palette(this);
    m_palettes.append(p);
    emit entrepotChanged();
    return p;
}

void Entrepot::supprimerPalette(Palette *p)
{
    if (!p) return;
    m_palettes.removeAll(p);
    p->deleteLater();
    emit entrepotChanged();
}

ReglesCompatibilite* Entrepot::regles()
{
    return &m_regles;
}

// ----------------------- FIFO (First In First Out) ----------------------------
void Entrepot::genererPalettesFIFO(double capacitePalette)
{
    QList<Product*> produits = tousLesProduits();

    // tri par date d’entrée (même si vide pour non périssables → ils iront en fin ou au début selon implémentation)
    std::sort(produits.begin(), produits.end(),
              [](Product *a, Product *b) {
                  return a->dateEntreeStock() < b->dateEntreeStock();
              });

    // reset palettes
    qDeleteAll(m_palettes);
    m_palettes.clear();

    Palette *palette = creerPalette();
    palette->setCapaciteMax(capacitePalette);

    for (Product *p : produits)
    {
        if (!p) continue;

        if (!palette->ajouterProduit(p, &m_regles))
        {
            palette = creerPalette();
            palette->setCapaciteMax(capacitePalette);
            palette->ajouterProduit(p, &m_regles);
        }
    }

    emit entrepotChanged();
}

// ----------------------- FEFO (First Expired First Out) ----------------------------
void Entrepot::genererPalettesFEFO(double capacitePalette)
{
    QList<Product*> produits;

    // 🎯 FEFO = uniquement les périssables
    for (Product *p : tousLesProduits())
    {
        if (qobject_cast<ProduitAvecCycleDeVie*>(p))
            produits.append(p);
    }

    // tri par date de péremption
    std::sort(produits.begin(), produits.end(),
              [](Product *a, Product *b) {
                  return a->datePeremption() < b->datePeremption();
              });

    // reset palettes
    qDeleteAll(m_palettes);
    m_palettes.clear();

    Palette *palette = creerPalette();
    palette->setCapaciteMax(capacitePalette);

    for (Product *p : produits)
    {
        if (!p) continue;

        if (!palette->ajouterProduit(p, &m_regles))
        {
            palette = creerPalette();
            palette->setCapaciteMax(capacitePalette);
            palette->ajouterProduit(p, &m_regles);
        }
    }

    emit entrepotChanged();
}
