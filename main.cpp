#include <QApplication>
#include <QTreeView>

#include "domain/warehouse.h"
#include "domain/container.h"
#include "domain/palette.h"
#include "domain/product.h"
#include "domain/produit_caracteristiques.h"
#include "domain/produit_cycledevie.h"
#include "model/warehousemodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // ========= 1) Entrepôt =========
    Entrepot *entrepot = new Entrepot();
    entrepot->setIdEntrepot("E1");
    entrepot->setNom("Entrepôt de test");

    // ========= 2) Conteneurs =========
    // C1
    Conteneur *c1 = entrepot->creerConteneur();
    c1->setIdConteneur("C1");
    c1->setCapaciteMax(1000);

    // C2
    Conteneur *c2 = entrepot->creerConteneur();
    c2->setIdConteneur("C2");
    c2->setCapaciteMax(800);

    // ========= 3) Produits dans les conteneurs =========
    // Produit1 dans C1 (non périssable)
    Product *prod1 = entrepot->creerProduitDans(c1, TypeProduit::Autre);
    prod1->setNom("Produit1");
    // si tu veux : qobject_cast<ProduitAvecCaracteristiques*>(prod1)->setPoids(10);

    // Produit2 dans C1 (périssable)
    Product *prod2 = entrepot->creerProduitDans(c1, TypeProduit::Alimentaire);
    prod2->setNom("Produit2");
    if (auto *p2 = qobject_cast<ProduitAvecCycleDeVie*>(prod2)) {
        p2->setDateEntreeStock(QDate::currentDate().addDays(-2));
        p2->setDatePeremption(QDate::currentDate().addDays(5));
    }

    // Produit3 dans C2 (non périssable)
    Product *prod3 = entrepot->creerProduitDans(c2, TypeProduit::Autre);
    prod3->setNom("Produit3");

    // ========= 4) Palettes =========
    Palette *pal1 = entrepot->creerPalette();
    pal1->setIdPalette("P1");
    pal1->setCapaciteMax(500);

    Palette *pal2 = entrepot->creerPalette();
    pal2->setIdPalette("P2");
    pal2->setCapaciteMax(500);

    // Palette P1 contient Produit1 et Produit3
    pal1->ajouterProduit(prod1, entrepot->regles());
    pal1->ajouterProduit(prod3, entrepot->regles());

    // Palette P2 contient Produit2
    pal2->ajouterProduit(prod2, entrepot->regles());

    // ========= 5) Modèle Qt =========
    WarehouseModel *model = new WarehouseModel(entrepot);

    // ========= 6) Vue =========
    QTreeView *view = new QTreeView();
    view->setModel(model);
    view->setWindowTitle("Structure Entrepôt");
    view->setHeaderHidden(true);
    //view->expandAll();          // pour voir toute la structure au début
    view->resize(600, 400);
    view->show();

    return app.exec();
}
