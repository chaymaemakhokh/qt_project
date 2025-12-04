// warehouse.h
#pragma once

#include <QObject>
#include <QList>
#include "container.h"
#include "palette.h"
#include "compatibility.h"

/**
 * @brief Entrepôt : racine du modèle, contient conteneurs, produits et palettes.
 */
class Entrepot : public QObject
{
    Q_OBJECT
public:
    explicit Entrepot(QObject *parent = nullptr);

    QString idEntrepot() const;
    void setIdEntrepot(const QString &id);

    QString nom() const;
    void setNom(const QString &n);

    QString adresse() const;
    void setAdresse(const QString &a);

    double surface() const;
    void setSurface(double s);

    const QList<Conteneur*>& conteneurs() const;
    const QList<Palette*>& palettes() const;

    QList<Product*> tousLesProduits() const;

    Conteneur* creerConteneur();
    void supprimerConteneur(Conteneur *c);

    Product* creerProduitDans(Conteneur *c);
    void supprimerProduitDe(Conteneur *c, Product *p);

    Palette* creerPalette();
    void supprimerPalette(Palette *p);

    ReglesCompatibilite* regles();

    // Construction automatique de palettes
    void genererPalettesFIFO(double capacitePalette);
    void genererPalettesFEFO(double capacitePalette);

signals:
    void entrepotChanged();

private:
    QString m_idEntrepot;
    QString m_nom;
    QString m_adresse;
    double m_surface {0.0};

    QList<Conteneur*> m_conteneurs;
    QList<Palette*> m_palettes;

    ReglesCompatibilite m_regles;
};
