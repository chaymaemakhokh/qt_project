#ifndef AJOUTERPRODUIT_H
#define AJOUTERPRODUIT_H

#include <QDialog>
#include "./domain/enums.h"

class ProduitControleur;
class ConteneurControleur;

namespace Ui {
class ajouterProduit;
}

class ajouterProduit : public QDialog
{
    Q_OBJECT

public:
    explicit ajouterProduit(ProduitControleur *prodCtrl,
                            ConteneurControleur *contCtrl,
                            QWidget *parent = nullptr);
    ~ajouterProduit();

private slots:
    void onCaracteristiquesToggled(bool checked);
    void onCycleDeVieToggled(bool checked);

    void onIdAutoToggled(bool checked);

    void on_Annuler_clicked();
    void on_Creer_clicked();

private:
    void updateTypeUI();
    void remplirComboConteneurs();


    bool validerIdManuel(QString &idOut);
    QString getOrCreateId();

    Ui::ajouterProduit *ui;
    ProduitControleur   *m_ctrl;
    ConteneurControleur *m_conteneurCtrl;
};

#endif // AJOUTERPRODUIT_H
