#include "ajouterproduit.h"
#include "ui_ajouterproduit.h"
#include "./domain/enums.h"
#include "./controlleur/produitcontroleur.h"
#include "./controlleur/conteneurcontroleur.h"
#include <QVariant>


ajouterProduit::ajouterProduit(ProduitControleur *prodCtrl,
                               ConteneurControleur *contCtrl,
                               QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ajouterProduit)
    , m_ctrl(prodCtrl)
    , m_conteneurCtrl(contCtrl)
{
    ui->setupUi(this);

    // état initial des checkBox (comme tu avais)
    ui->checkBox->setChecked(true);
    ui->checkBox_2->setChecked(false);

    connect(ui->checkBox,   &QCheckBox::toggled,
            this,           &ajouterProduit::onCaracteristiquesToggled);
    connect(ui->checkBox_2, &QCheckBox::toggled,
            this,           &ajouterProduit::onCycleDeVieToggled);

    // remplir TypeProduit
    ui->TypeProduit_2->clear();
    for (TypeProduit t : {TypeProduit::Alimentaire,
                          TypeProduit::Electronique,
                          TypeProduit::Medicament,
                          TypeProduit::Autre}) {
        ui->TypeProduit_2->addItem(toString(t), QVariant::fromValue(t));
    }

    // remplir EtatProduit
    ui->Etat_2->clear();
    for (EtatProduit e : {EtatProduit::Stocke, EtatProduit::Expedie}) {
        ui->Etat_2->addItem(toString(e), QVariant::fromValue(e));
    }

    // remplir la combo des conteneurs
    remplirComboConteneurs();

    updateTypeUI();
}

void ajouterProduit::remplirComboConteneurs()
{
    ui->DansQuelConteneur_2->clear();

    // Optionnel : première ligne = "aucun"
    ui->DansQuelConteneur_2->addItem(QString());  // item vide

    if (!m_conteneurCtrl) return;

    const auto &list = m_conteneurCtrl->conteneurs();
    for (const auto &c : list) {
        if (!c) continue;
        // on met l'id comme texte ET comme data
        ui->DansQuelConteneur_2->addItem(c->idConteneur(), c->idConteneur());
    }
}

ajouterProduit::~ajouterProduit()
{
    delete ui;
}

// ========================
//   Slots de gestion UI
// ========================

void ajouterProduit::onCaracteristiquesToggled(bool checked)
{
    // On veut toujours exactement UN type sélectionné.
    if (!checked && !ui->checkBox_2->isChecked()) {
        // Empêcher l'état "aucun des deux cochés"
        ui->checkBox->blockSignals(true);
        ui->checkBox->setChecked(true);
        ui->checkBox->blockSignals(false);
        return;
    }

    if (checked) {
        // Exclusivité : si celui-ci est coché, on décoche l'autre.
        ui->checkBox_2->blockSignals(true);
        ui->checkBox_2->setChecked(false);
        ui->checkBox_2->blockSignals(false);
    }

    updateTypeUI();
}

void ajouterProduit::onCycleDeVieToggled(bool checked)
{
    if (!checked && !ui->checkBox->isChecked()) {
        // Empêcher l'état "aucun des deux cochés"
        ui->checkBox_2->blockSignals(true);
        ui->checkBox_2->setChecked(true);
        ui->checkBox_2->blockSignals(false);
        return;
    }

    if (checked) {
        ui->checkBox->blockSignals(true);
        ui->checkBox->setChecked(false);
        ui->checkBox->blockSignals(false);
    }

    updateTypeUI();
}

// ========================
//   Mise à jour d'affichage
// ========================

void ajouterProduit::updateTypeUI()
{
    const bool caracteristiques = ui->checkBox->isChecked();
    const bool cycleDeVie       = ui->checkBox_2->isChecked();

    // Produit avec caractéristiques :
    //  -> on AFFICHE ConditionsConservation
    //  -> on CACHE DatePeremption
    ui->ConditionsConservation->setVisible(caracteristiques);
    ui->ConditionsConservation_2->setVisible(caracteristiques);

    // Produit avec cycle de vie :
    //  -> on CACHE ConditionsConservation
    //  -> on AFFICHE DatePeremption
    ui->DatePeremption->setVisible(cycleDeVie);
    ui->DatePeremption_2->setVisible(cycleDeVie);
}

void ajouterProduit::on_Annuler_clicked()
{
    this->close();
}


void ajouterProduit::on_Creer_clicked()
{
    if (!m_ctrl) {
        accept();
        return;
    }

    QString id   = ui->IdProduit_2->text();
    QString nom  = ui->NomProduit_2->text();

    double capaciteMax = ui->CapaciteMax_2->value();
    double poids       = ui->Poids_2->value();
    double volume      = ui->Volume_2->value();

    QDate dateEntree = ui->DateEntreeStock_2->date();

    TypeProduit type = ui->TypeProduit_2->currentData().value<TypeProduit>();
    EtatProduit etat = ui->Etat_2->currentData().value<EtatProduit>();

    bool isCaracteristiques = ui->checkBox->isChecked();
    bool isCycleDeVie       = ui->checkBox_2->isChecked();

    // ID du conteneur choisi (peut être vide)
    QString idConteneur = ui->DansQuelConteneur_2->currentData().toString();
    if (idConteneur.isEmpty())
        idConteneur = ui->DansQuelConteneur_2->currentText();

    std::shared_ptr<Product> produit;

    if (isCaracteristiques) {
        QString conditions = ui->ConditionsConservation_2->toPlainText();

        produit = m_ctrl->ajouterProduitAvecCaracteristiques(
            id, nom, type, capaciteMax,
            poids, volume,
            conditions,
            dateEntree,
            etat
            );
    }
    else if (isCycleDeVie) {
        QDate datePeremption = ui->DatePeremption_2->date();

        produit = m_ctrl->ajouterProduitAvecCycleDeVie(
            id, nom, type, capaciteMax,
            poids, volume,
            dateEntree,
            datePeremption,
            etat
            );
    }

    // si un conteneur est choisi, on y ajoute le produit
    if (produit && m_conteneurCtrl && !idConteneur.isEmpty()) {
        m_conteneurCtrl->ajouterProduitAuConteneur(idConteneur, produit);
    }

    accept();
}

