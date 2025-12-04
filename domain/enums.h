#pragma once

#include <QString>

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

// Helpers simples (implémentés dans enums.cpp)
QString toString(TypeProduit t);
QString toString(TypeConteneur t);
QString toString(EtatProduit e);

TypeProduit typeProduitFromString(const QString &s);
TypeConteneur typeConteneurFromString(const QString &s);
EtatProduit etatProduitFromString(const QString &s);
