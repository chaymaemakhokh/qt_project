#include "persistence.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDate>

#include "./controlleur/conteneurcontroleur.h"
#include "./controlleur/produitcontroleur.h"
#include "./controlleur/palettecontroleur.h"

#include "./domain/container.h"
#include "./domain/product.h"
#include "./domain/produit_caracteristiques.h"
#include "./domain/produit_cycledevie.h"
#include "./domain/palette.h"
#include "./domain/enums.h"

#include <memory>
#include <QMap>

using std::shared_ptr;

namespace Persistence {

// ------------------------------------------------------
//  EXPORT
// ------------------------------------------------------
bool exporterJSON(const QString &fileName,
                  const ConteneurControleur *contCtrl,
                  const ProduitControleur   *prodCtrl,
                  const PaletteControleur   *palCtrl)
{
    QJsonObject root;

    // ---------- PRODUITS ----------
    QJsonArray produitsArray;
    if (prodCtrl) {
        const auto &prods = prodCtrl->produits();
        for (const auto &sp : prods) {
            if (!sp) continue;
            Product *p = sp.get();

            QJsonObject obj;
            obj["id"]           = p->idProduit();
            obj["nom"]          = p->nom();
            obj["type"]         = toString(p->type());
            obj["capaciteMax"]  = p->capaciteMax();
            obj["etat"]         = toString(p->etat());
            obj["poids"]        = p->poids();
            obj["volume"]       = p->volume();
            obj["dateEntree"]   = p->dateEntreeStock().toString(Qt::ISODate);

            // On distingue les 2 sous-types
            if (auto pc = qobject_cast<ProduitAvecCaracteristiques*>(p)) {
                obj["kind"] = "Caracteristiques";
                obj["conditionsConservation"] = pc->conditionsConservation();
            } else if (auto pv = qobject_cast<ProduitAvecCycleDeVie*>(p)) {
                obj["kind"] = "CycleDeVie";
                obj["datePeremption"] =
                    pv->datePeremption().toString(Qt::ISODate);
            } else {
                obj["kind"] = "Inconnu";
            }

            produitsArray.append(obj);
        }
    }
    root["produits"] = produitsArray;

    // ---------- CONTENEURS ----------
    QJsonArray contsArray;
    if (contCtrl) {
        const auto &conts = contCtrl->conteneurs();
        for (const auto &sc : conts) {
            if (!sc) continue;
            Conteneur *c = sc.get();

            QJsonObject obj;
            obj["id"]          = c->idConteneur();
            obj["type"]        = toString(c->type());
            obj["capaciteMax"] = c->capaciteMax();

            // liste des ids de produits présents dans ce conteneur
            QJsonArray prodIds;
            for (Product *p : c->produits()) {
                if (p)
                    prodIds.append(p->idProduit());
            }
            obj["produitsIds"] = prodIds;

            contsArray.append(obj);
        }
    }
    root["conteneurs"] = contsArray;

    // ---------- PALETTES ----------
    QJsonArray palsArray;
    if (palCtrl) {
        const auto &pals = palCtrl->palettes();
        for (const auto &sp : pals) {
            if (!sp) continue;
            Palette *p = sp.get();

            QJsonObject obj;
            obj["id"]          = p->idPalette();
            obj["destination"] = p->destination();
            obj["dateEnvoiPrevue"] =
                p->dateEnvoiPrevue().toString(Qt::ISODate);
            obj["capaciteMax"] = p->capaciteMax();

            // (pour l’instant, on n’exporte pas les ElementsPalette,
            //  tu pourras les rajouter plus tard si besoin)

            palsArray.append(obj);
        }
    }
    root["palettes"] = palsArray;

    // ---------- ÉCRITURE FICHIER ----------
    QJsonDocument doc(root);
    QFile f(fileName);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    f.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

// ------------------------------------------------------
//  IMPORT
// ------------------------------------------------------
bool importerJSON(const QString &fileName,
                  ConteneurControleur *contCtrl,
                  ProduitControleur   *prodCtrl,
                  PaletteControleur   *palCtrl)
{
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    const QByteArray data = f.readAll();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject())
        return false;

    QJsonObject root = doc.object();

    // On commence par VIDER les contrôleurs
    if (contCtrl) contCtrl->clear();
    if (prodCtrl) prodCtrl->clear();
    if (palCtrl) palCtrl->clear();

    // ---------- PRODUITS ----------
    QMap<QString, shared_ptr<Product>> mapProd; // id -> produit

    if (prodCtrl) {
        QJsonArray produitsArray = root["produits"].toArray();
        for (const QJsonValue &val : produitsArray) {
            QJsonObject obj = val.toObject();

            QString id  = obj["id"].toString();
            QString nom = obj["nom"].toString();
            QString typeStr = obj["type"].toString();
            TypeProduit type = typeProduitFromString(typeStr);
            double capaciteMax = obj["capaciteMax"].toDouble();
            QString etatStr = obj["etat"].toString();
            EtatProduit etat = etatProduitFromString(etatStr);
            double poids  = obj["poids"].toDouble();
            double volume = obj["volume"].toDouble();
            QDate dateEntree =
                QDate::fromString(obj["dateEntree"].toString(), Qt::ISODate);

            QString kind = obj["kind"].toString();

            shared_ptr<Product> p;
            if (kind == "CycleDeVie") {
                QDate datePer =
                    QDate::fromString(obj["datePeremption"].toString(),
                                      Qt::ISODate);
                p = prodCtrl->ajouterProduitAvecCycleDeVie(
                    id, nom, type, capaciteMax,
                    poids, volume,
                    dateEntree, datePer,
                    etat);
            } else {
                QString conds = obj["conditionsConservation"].toString();
                p = prodCtrl->ajouterProduitAvecCaracteristiques(
                    id, nom, type, capaciteMax,
                    poids, volume,
                    conds,
                    dateEntree,
                    etat);
            }

            if (!id.isEmpty() && p) {
                mapProd.insert(id, p);
            }
        }
    }

    // ---------- CONTENEURS ----------
    if (contCtrl) {
        QJsonArray contsArray = root["conteneurs"].toArray();
        for (const QJsonValue &val : contsArray) {
            QJsonObject obj = val.toObject();

            QString id = obj["id"].toString();
            QString typeStr = obj["type"].toString();
            TypeConteneur type = typeConteneurFromString(typeStr);
            double capaciteMax = obj["capaciteMax"].toDouble();

            contCtrl->ajouterConteneur(id, type, capaciteMax);

            // Re-lier les produits dans ce conteneur
            QJsonArray prodIds = obj["produitsIds"].toArray();
            for (const QJsonValue &vId : prodIds) {
                QString pid = vId.toString();
                if (mapProd.contains(pid)) {
                    contCtrl->ajouterProduitAuConteneur(id, mapProd[pid]);
                }
            }
        }
    }

    // ---------- PALETTES ----------
    if (palCtrl) {
        QJsonArray palsArray = root["palettes"].toArray();
        for (const QJsonValue &val : palsArray) {
            QJsonObject obj = val.toObject();
            QString id    = obj["id"].toString();
            QString dest  = obj["destination"].toString();
            QDate dEnvoi  =
                QDate::fromString(obj["dateEnvoiPrevue"].toString(),
                                  Qt::ISODate);
            double capMax = obj["capaciteMax"].toDouble();
            palCtrl->ajouterPalette(id, dest, dEnvoi, capMax);
        }
    }

    return true;
}

} // namespace Persistence
