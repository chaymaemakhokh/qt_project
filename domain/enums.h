#pragma once

#include <QString>
#include <QMetaType>

enum class TypeProduit {
    Alimentaire,
    Electronique,
    Medicament,
    Autre
};

enum class TypeConteneur {
    Normal,
    Froid,
    Fragile,
    Autre
};

enum class EtatProduit {
    Stocke,
    Expedie
};

// ⚠️ Pas de ; à la fin
Q_DECLARE_METATYPE(TypeProduit)
Q_DECLARE_METATYPE(TypeConteneur)
Q_DECLARE_METATYPE(EtatProduit)

// Helpers simples (implémentés dans enums.cpp)
QString toString(TypeProduit t);
QString toString(TypeConteneur t);
QString toString(EtatProduit e);

TypeProduit typeProduitFromString(const QString &s);
TypeConteneur typeConteneurFromString(const QString &s);
EtatProduit etatProduitFromString(const QString &s);
