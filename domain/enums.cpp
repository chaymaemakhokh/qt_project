
#include "enums.h"

QString toString(TypeProduit t)
{
    switch (t) {
    case TypeProduit::Alimentaire: return "ALIMENTAIRE";
    case TypeProduit::Electronique: return "ELECTRONIQUE";
    case TypeProduit::Medicament:   return "MEDICAMENT";
    case TypeProduit::Autre:        return "AUTRE";
    }
    return "AUTRE";
}

QString toString(TypeConteneur t)
{
    switch (t) {
    case TypeConteneur::Normal:  return "NORMAL";
    case TypeConteneur::Froid:   return "FROID";
    case TypeConteneur::Fragile: return "FRAGILE";
    case TypeConteneur::Autre:   return "AUTRE";
    }
    return "AUTRE";
}

QString toString(EtatProduit e)
{
    switch (e) {
    case EtatProduit::Stocke:  return "STOCKE";
    case EtatProduit::Expedie: return "EXPEDIE";
    }
    return "STOCKE";
}

TypeProduit typeProduitFromString(const QString &s)
{
    if (s == "ALIMENTAIRE") return TypeProduit::Alimentaire;
    if (s == "ELECTRONIQUE") return TypeProduit::Electronique;
    if (s == "MEDICAMENT")   return TypeProduit::Medicament;
    return TypeProduit::Autre;
}

TypeConteneur typeConteneurFromString(const QString &s)
{
    if (s == "NORMAL")  return TypeConteneur::Normal;
    if (s == "FROID")   return TypeConteneur::Froid;
    if (s == "FRAGILE") return TypeConteneur::Fragile;
    return TypeConteneur::Autre;
}

EtatProduit etatProduitFromString(const QString &s)
{
    if (s == "STOCKE")  return EtatProduit::Stocke;
    if (s == "EXPEDIE") return EtatProduit::Expedie;
    return EtatProduit::Stocke;
}
