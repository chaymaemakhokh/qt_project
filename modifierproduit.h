#ifndef MODIFIERPRODUIT_H
#define MODIFIERPRODUIT_H

#include <QDialog>
#include <memory>

#include "./domain/enums.h"

class ProduitControleur;
class ConteneurControleur;
class Product;

namespace Ui {
class modifierproduit;
}

class modifierproduit : public QDialog
{
    Q_OBJECT

public:
    explicit modifierproduit(const QString &idProduit,
                             ProduitControleur *prodCtrl,
                             ConteneurControleur *contCtrl,
                             QWidget *parent = nullptr);
    ~modifierproduit();

private slots:
    void on_Modifier_clicked();
    void on_Annuler_clicked();

private:
    void remplirCombos();
    void chargerProduitDansUI();
    bool appliquerModifsDepuisUI();

private:
    Ui::modifierproduit *ui;

    QString m_idProduit;
    ProduitControleur *m_prodCtrl {nullptr};
    ConteneurControleur *m_contCtrl {nullptr};
};

#endif // MODIFIERPRODUIT_H
