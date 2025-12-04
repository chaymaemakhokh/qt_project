// warehouse.cpp
#include "warehouse.h"
#include "product.h"
#include <algorithm>

Entrepot::Entrepot(QObject *parent)
    : QObject(parent)
{
}

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

const QList<Conteneur *> &Entrepot::conteneurs() const { return m_conteneurs; }
const QList<Palette *> &Entrepot::palettes() const { return m_palettes; }

QList<Product *> Entrepot::tousLesProduits() const
{
    QList<Product*> result;
    for (Conteneur *c : m_conteneurs) {
        if (!c) continue;
        for (Product *p : c->produits()) {
            if (p && !result.contains(p))
                result.append(p);
        }
    }
    return result;
}

Conteneur *Entrepot::creerConteneur()
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

Product *Entrepot::creerProduitDans(Conteneur *c)
{
    if (!c) return nullptr;
    auto *p = new Product(c);
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

Palette *Entrepot::creerPalette()
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

ReglesCompatibilite *Entrepot::regles()
{
    return &m_regles;
}

// --------- Algorithmes FIFO & FEFO ---------

void Entrepot::genererPalettesFIFO(double capacitePalette)
{
    // récupérer tous les produits stockés
    QList<Product*> produits = tousLesProduits();
    std::sort(produits.begin(), produits.end(),
              [](Product *a, Product *b) {
                  return a->dateEntreeStock() < b->dateEntreeStock();
              });

    // vider les palettes existantes
    qDeleteAll(m_palettes);
    m_palettes.clear();

    Palette *courante = creerPalette();
    courante->setCapaciteMax(capacitePalette);

    for (Product *p : produits) {
        if (!p) continue;
        if (!courante->ajouterProduit(p, &m_regles)) {
            courante = creerPalette();
            courante->setCapaciteMax(capacitePalette);
            courante->ajouterProduit(p, &m_regles);
        }
    }

    emit entrepotChanged();
}

void Entrepot::genererPalettesFEFO(double capacitePalette)
{
    QList<Product*> produits = tousLesProduits();
    std::sort(produits.begin(), produits.end(),
              [](Product *a, Product *b) {
                  return a->datePeremption() < b->datePeremption();
              });

    qDeleteAll(m_palettes);
    m_palettes.clear();

    Palette *courante = creerPalette();
    courante->setCapaciteMax(capacitePalette);

    for (Product *p : produits) {
        if (!p) continue;
        if (!courante->ajouterProduit(p, &m_regles)) {
            courante = creerPalette();
            courante->setCapaciteMax(capacitePalette);
            courante->ajouterProduit(p, &m_regles);
        }
    }

    emit entrepotChanged();
}
