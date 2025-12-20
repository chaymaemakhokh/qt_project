#ifndef PALETTECONTROLEUR_H
#define PALETTECONTROLEUR_H

#pragma once

#include <QObject>
#include <QVector>
#include <memory>

#include "./domain/palette.h"

struct FiltrePalette
{
    QString idPartiel;              // vide = pas de filtre
    QString destinationPartielle;   // vide = pas de filtre

    bool  filtrerParDateEnvoi = false;
    QDate dateEnvoi;

    bool   filtrerParCapacite = false;
    double capaciteMax = 0.0;
};

class PaletteControleur : public QObject
{
    Q_OBJECT
public:
    explicit PaletteControleur(QObject *parent = nullptr);

    void ajouterPalette(const QString &id,
                        const QString &destination,
                        const QDate  &dateEnvoiPrevue,
                        double capaciteMax);

    const QVector<std::shared_ptr<Palette>>& palettes() const
    {
        return m_palettes;
    }

    QVector<std::shared_ptr<Palette>>
    rechercherParIdPartiel(const QString &idPartiel) const;

    QVector<std::shared_ptr<Palette>>
    rechercherParFiltre(const FiltrePalette &filtre) const;

    Palette* trouverPaletteParId(const QString &id) const;

    bool supprimerPaletteParId(const QString &id);

    void debugPrintPalettes() const;
    void vider();
    void genererPalettesAutomatiquement(
        const QVector<std::shared_ptr<Product>> &produits,
        const ReglesCompatibilite *regles);

    bool idExiste(const QString &id) const;
    QString genererIdPaletteUnique(int longueur = 8) const;


private:
    QVector<std::shared_ptr<Palette>> m_palettes;
};



#endif // PALETTECONTROLEUR_H
