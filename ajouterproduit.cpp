#include "ajouterproduit.h"
#include "ui_ajouterproduit.h"
#include "./domain/enums.h"
#include "./controlleur/produitcontroleur.h"
#include "./controlleur/conteneurcontroleur.h"
#include <QVariant>
#include <QCheckBox>
#include <QMessageBox>


ajouterProduit::ajouterProduit(ProduitControleur *prodCtrl,
                               ConteneurControleur *contCtrl,
                               QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ajouterProduit)
    , m_ctrl(prodCtrl)
    , m_conteneurCtrl(contCtrl)
{
    ui->setupUi(this);

    ui->DateEntreeStock_2->setDate(QDate::currentDate());
    ui->DatePeremption_2->setDate(QDate::currentDate().addYears(1));

    // état initial des checkBox (comme tu avais)
    ui->checkBox->setChecked(true);
    ui->checkBox_2->setChecked(false);

    connect(ui->checkBox,   &QCheckBox::toggled,
            this,           &ajouterProduit::onCaracteristiquesToggled);
    connect(ui->checkBox_2, &QCheckBox::toggled,
            this,           &ajouterProduit::onCycleDeVieToggled);


    if (ui->checkBox_Id_Automatique) {
        ui->checkBox_Id_Automatique->setChecked(true);
        connect(ui->checkBox_Id_Automatique, &QCheckBox::toggled,
                this, &ajouterProduit::onIdAutoToggled);
        onIdAutoToggled(true);
    }

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

void ajouterProduit::onIdAutoToggled(bool checked)
{
    // si auto : on désactive la saisie manuelle
    ui->IdProduit_2->setEnabled(!checked);

    if (checked) {
        ui->IdProduit_2->clear();
        ui->IdProduit_2->setPlaceholderText("ID généré automatiquement");
    } else {
        ui->IdProduit_2->setPlaceholderText("Ex: PABC123");
        ui->IdProduit_2->setFocus();
    }
}

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

    // ✅ Avec stackedWidget : on change juste de page
    if (caracteristiques) {
        ui->stackedWidget->setCurrentWidget(ui->page_Caracteristiques);
    } else {
        ui->stackedWidget->setCurrentWidget(ui->page_CycleDeVie);
    }
}

void ajouterProduit::on_Annuler_clicked()
{
    this->close();
}

bool ajouterProduit::validerIdManuel(QString &idOut)
{
    idOut = ui->IdProduit_2->text().trimmed();

    if (idOut.isEmpty()) {
        QMessageBox::warning(this, tr("ID invalide"),
                             tr("Veuillez saisir un ID produit."));
        return false;
    }

    if (m_ctrl && m_ctrl->idExiste(idOut)) {
        QMessageBox::warning(this, tr("ID déjà utilisé"),
                             tr("L'ID \"%1\" existe déjà.\nVeuillez en choisir un autre.")
                                 .arg(idOut));
        ui->IdProduit_2->setFocus();
        ui->IdProduit_2->selectAll();
        return false;
    }

    return true;
}

QString ajouterProduit::getOrCreateId()
{
    const bool autoId = (ui->checkBox_Id_Automatique && ui->checkBox_Id_Automatique->isChecked());

    if (autoId) {
        // ✅ génère un ID unique garanti
        return m_ctrl ? m_ctrl->genererIdProduitUnique(8) : QString();
    }

    QString manual;
    if (!validerIdManuel(manual))
        return QString();

    return manual;
}


void ajouterProduit::on_Creer_clicked()
{
    if (!m_ctrl) { accept(); return; }

    // ---------------------------
    // 1) ID auto / manuel + unicité
    // ---------------------------
    const bool idAuto = (ui->checkBox_Id_Automatique && ui->checkBox_Id_Automatique->isChecked());

    QString id;
    if (idAuto) {
        id = m_ctrl->genererIdProduitUnique(8);   // P + 8 caractères
        // Optionnel : afficher l'id généré dans le champ (même s'il est disabled)
        ui->IdProduit_2->setText(id);
    } else {
        id = ui->IdProduit_2->text().trimmed();

        if (id.isEmpty()) {
            QMessageBox::warning(this, tr("ID invalide"),
                                 tr("Veuillez saisir un ID produit."));
            return;
        }

        if (m_ctrl->idExiste(id)) {
            QMessageBox::warning(this, tr("ID déjà utilisé"),
                                 tr("L'ID \"%1\" existe déjà.\nVeuillez en choisir un autre.")
                                     .arg(id));
            ui->IdProduit_2->setFocus();
            ui->IdProduit_2->selectAll();
            return;
        }
    }

    // ---------------------------
    // 2) Récupération des champs
    // ---------------------------
    QString nom = ui->NomProduit_2->text().trimmed();
    if (nom.isEmpty()) {
        QMessageBox::warning(this, tr("Nom invalide"),
                             tr("Veuillez saisir le nom du produit."));
        return;
    }

    double capaciteMax = ui->CapaciteMax_2->value();   // ✅ capacité du produit
    double poids       = ui->Poids_2->value();
    double volume      = ui->Volume_2->value();

    QDate dateEntree = ui->DateEntreeStock_2->date();

    TypeProduit type = ui->TypeProduit_2->currentData().value<TypeProduit>();
    EtatProduit etat = ui->Etat_2->currentData().value<EtatProduit>();

    // conteneur choisi
    QString idConteneur = ui->DansQuelConteneur_2->currentData().toString().trimmed();
    if (idConteneur.isEmpty())
        idConteneur = ui->DansQuelConteneur_2->currentText().trimmed();

    // ---------------------------
    // 3) Vérification capacité AVANT création (pour les 2 pages)
    // ---------------------------
    if (!idConteneur.isEmpty() && m_conteneurCtrl) {
        const bool okCap = m_conteneurCtrl->peutAjouterProduitAuConteneur(idConteneur, capaciteMax);
        if (!okCap) {
            Conteneur *c = m_conteneurCtrl->trouverConteneurParId(idConteneur);
            const double restante = c ? c->capaciteRestante() : 0.0;

            QMessageBox::warning(
                this,
                tr("Ajout impossible"),
                tr("Le conteneur %1 n'a pas assez de capacité.\n"
                   "Capacité restante = %2\n"
                   "Capacité produit = %3")
                    .arg(idConteneur)
                    .arg(restante)
                    .arg(capaciteMax)
                );
            return; // ❌ on ne crée pas le produit
        }
    }

    // ---------------------------
    // 4) Création du produit
    // ---------------------------
    std::shared_ptr<Product> produit;

    if (ui->checkBox->isChecked()) {
        // Caractéristiques
        QString conditions = ui->ConditionsConservation_2->toPlainText();

        produit = m_ctrl->ajouterProduitAvecCaracteristiques(
            id, nom, type, capaciteMax,
            poids, volume,
            conditions,
            dateEntree,
            etat
            );
    } else {
        // Cycle de vie
        QDate datePeremption = ui->DatePeremption_2->date();

        produit = m_ctrl->ajouterProduitAvecCycleDeVie(
            id, nom, type, capaciteMax,
            poids, volume,
            dateEntree,
            datePeremption,
            etat
            );
    }

    // ---------------------------
    // 5) Ajout au conteneur
    // ---------------------------
    if (produit && m_conteneurCtrl && !idConteneur.isEmpty()) {
        const bool okAjout = m_conteneurCtrl->ajouterProduitAuConteneur(idConteneur, produit);
        if (!okAjout) {
            // sécurité (normalement jamais si le check avant est bon)
            QMessageBox::warning(this, tr("Ajout impossible"),
                                 tr("Impossible d'ajouter le produit au conteneur %1.")
                                     .arg(idConteneur));
            return;
        }
    }

    accept();
}


