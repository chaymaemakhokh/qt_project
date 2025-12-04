// palette.h
#pragma once

#include <QObject>
#include <QList>
#include <QDate>
#include "elements_palette.h"
#include "compatibility.h"

class Product;

/**
 * @brief Palette d'expédition.
 */
class Palette : public QObject
{
    Q_OBJECT
public:
    explicit Palette(QObject *parent = nullptr);

    QString idPalette() const;
    void setIdPalette(const QString &id);

    QString destination() const;
    void setDestination(const QString &dest);

    QDate dateEnvoiPrevue() const;
    void setDateEnvoiPrevue(const QDate &d);

    double capaciteMax() const;
    void setCapaciteMax(double c);

    const QList<ElementsPalette>& elements() const;
    QList<ElementsPalette>& elementsRef(); // pour modification interne

    double poidsTotal() const;

    bool peutAjouter(Product *p, const ReglesCompatibilite *regles) const;
    bool ajouterProduit(Product *p, const ReglesCompatibilite *regles);

signals:
    void paletteChanged();

private:
    QString m_idPalette;
    QString m_destination;
    QDate m_dateEnvoiPrevue;
    double m_capaciteMax {0.0};
    QList<ElementsPalette> m_elements;
};
