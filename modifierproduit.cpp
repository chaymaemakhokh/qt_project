#include "modifierproduit.h"
#include "ui_modifierproduit.h"

#include "./controlleur/produitcontroleur.h"
#include "./controlleur/conteneurcontroleur.h"

#include "./domain/product.h"
#include "./domain/produit_caracteristiques.h"
#include "./domain/produit_cycledevie.h"

#include <QVariant>
#include <QMessageBox>

modifierproduit::modifierproduit(const QString &idProduit,
                                 ProduitControleur *prodCtrl,
                                 ConteneurControleur *contCtrl,
                                 QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::modifierproduit)
    , m_idProduit(idProduit.trimmed())
    , m_prodCtrl(prodCtrl)
    , m_contCtrl(contCtrl)
{
    ui->setupUi(this);

    remplirCombos();

    ui->IdProduit_2->setReadOnly(true);
    ui->IdProduit_2->setEnabled(false);

    ui->IdProduit_4->setReadOnly(true);
    ui->IdProduit_4->setEnabled(false);

    chargerProduitDansUI();
}

modifierproduit::~modifierproduit()
{
    delete ui;
}

void modifierproduit::remplirCombos()
{
    // Page caractéristiques
    ui->TypeProduit_2->clear();
    for (TypeProduit t : {TypeProduit::Alimentaire, TypeProduit::Electronique,
                          TypeProduit::Medicament, TypeProduit::Autre}) {
        ui->TypeProduit_2->addItem(toString(t), QVariant::fromValue(t));
    }

    ui->Etat_2->clear();
    for (EtatProduit e : {EtatProduit::Stocke, EtatProduit::Expedie}) {
        ui->Etat_2->addItem(toString(e), QVariant::fromValue(e));
    }

    // Page cycle de vie
    ui->TypeProduit_4->clear();
    for (TypeProduit t : {TypeProduit::Alimentaire, TypeProduit::Electronique,
                          TypeProduit::Medicament, TypeProduit::Autre}) {
        ui->TypeProduit_4->addItem(toString(t), QVariant::fromValue(t));
    }

    ui->Etat_4->clear();
    for (EtatProduit e : {EtatProduit::Stocke, EtatProduit::Expedie}) {
        ui->Etat_4->addItem(toString(e), QVariant::fromValue(e));
    }
}

static void setComboByData(QComboBox *cb, const QVariant &data)
{
    if (!cb) return;
    for (int i = 0; i < cb->count(); ++i) {
        if (cb->itemData(i) == data) {
            cb->setCurrentIndex(i);
            return;
        }
    }
}

void modifierproduit::chargerProduitDansUI()
{
    if (!m_prodCtrl || m_idProduit.isEmpty()) return;

    Product *p = m_prodCtrl->trouverProduitParId(m_idProduit);
    if (!p) {
        QMessageBox::warning(this, tr("Erreur"),
                             tr("Produit introuvable (id=%1).").arg(m_idProduit));
        reject();
        return;
    }

    // Détecter le type réel
    auto *pc = dynamic_cast<ProduitAvecCaracteristiques*>(p);
    auto *pv = dynamic_cast<ProduitAvecCycleDeVie*>(p);

    if (pc) {
        ui->stackedWidget->setCurrentWidget(ui->page_ProduitAvecCaracteristiques);

        ui->IdProduit_2->setText(pc->idProduit());
        ui->NomProduit_2->setText(pc->nom());
        setComboByData(ui->TypeProduit_2, QVariant::fromValue(pc->type()));
        ui->CapaciteMax_2->setValue(pc->capaciteMax());
        setComboByData(ui->Etat_2, QVariant::fromValue(pc->etat()));
        ui->Poids_2->setValue(pc->poids());
        ui->Volume_2->setValue(pc->volume());
        ui->DateEntreeStock_2->setDate(pc->dateEntreeStock());
        ui->ConditionsConservation_2->setPlainText(pc->conditionsConservation());
        return;
    }

    if (pv) {
        ui->stackedWidget->setCurrentWidget(ui->page_ProduitAvecCycleDeVie);

        ui->IdProduit_4->setText(pv->idProduit());
        ui->NomProduit_4->setText(pv->nom());
        setComboByData(ui->TypeProduit_4, QVariant::fromValue(pv->type()));
        ui->CapaciteMax_4->setValue(pv->capaciteMax());
        setComboByData(ui->Etat_4, QVariant::fromValue(pv->etat()));
        ui->Poids_4->setValue(pv->poids());
        ui->Volume_4->setValue(pv->volume());
        ui->DateEntreeStock_4->setDate(pv->dateEntreeStock());
        ui->DatePeremption_2->setDate(pv->datePeremption());
        return;
    }

    // Si jamais un autre type arrive
    QMessageBox::warning(this, tr("Erreur"),
                         tr("Type de produit non géré pour la modification."));
    reject();
}

bool modifierproduit::appliquerModifsDepuisUI()
{
    if (!m_prodCtrl || m_idProduit.isEmpty()) return false;

    Product *p = m_prodCtrl->trouverProduitParId(m_idProduit);
    if (!p) return false;

    auto *pc = dynamic_cast<ProduitAvecCaracteristiques*>(p);
    auto *pv = dynamic_cast<ProduitAvecCycleDeVie*>(p);

    // ✅ On bloque la modification si la nouvelle capacité casse un conteneur
    auto checkCapacite = [&](double nouvelleCap) -> bool {
        if (!m_contCtrl) return true;
        QString contBloquant;
        double restante = 0.0;
        const bool ok = m_contCtrl->peutModifierCapaciteProduit(m_idProduit, nouvelleCap,
                                                                &contBloquant, &restante);
        if (!ok) {
            QMessageBox::warning(
                this,
                tr("Modification impossible"),
                tr("La nouvelle capacité du produit dépasse la capacité disponible.\n"
                   "Conteneur bloquant : %1\n"
                   "Capacité restante : %2\n"
                   "Capacité demandée : %3")
                    .arg(contBloquant)
                    .arg(restante)
                    .arg(nouvelleCap)
                );
        }
        return ok;
    };

    if (pc) {
        const QString nom = ui->NomProduit_2->text().trimmed();
        const TypeProduit type = ui->TypeProduit_2->currentData().value<TypeProduit>();
        const EtatProduit etat = ui->Etat_2->currentData().value<EtatProduit>();

        const double capMax = ui->CapaciteMax_2->value();
        if (!checkCapacite(capMax)) return false;

        pc->setNom(nom);
        pc->setType(type);
        pc->setCapaciteMax(capMax);
        pc->setEtat(etat);
        pc->setPoids(ui->Poids_2->value());
        pc->setVolume(ui->Volume_2->value());
        pc->setDateEntreeStock(ui->DateEntreeStock_2->date());
        pc->setConditionsConservation(ui->ConditionsConservation_2->toPlainText());
        return true;
    }

    if (pv) {
        const QString nom = ui->NomProduit_4->text().trimmed();
        const TypeProduit type = ui->TypeProduit_4->currentData().value<TypeProduit>();
        const EtatProduit etat = ui->Etat_4->currentData().value<EtatProduit>();

        const double capMax = ui->CapaciteMax_4->value();
        if (!checkCapacite(capMax)) return false;

        pv->setNom(nom);
        pv->setType(type);
        pv->setCapaciteMax(capMax);
        pv->setEtat(etat);
        pv->setPoids(ui->Poids_4->value());
        pv->setVolume(ui->Volume_4->value());
        pv->setDateEntreeStock(ui->DateEntreeStock_4->date());
        pv->setDatePeremption(ui->DatePeremption_2->date());
        return true;
    }

    return false;
}

void modifierproduit::on_Modifier_clicked()
{
    const auto rep = QMessageBox::question(
        this,
        tr("Confirmer la modification"),
        tr("Vous êtes sûr(e) de vouloir modifier ce produit ?"),
        QMessageBox::Yes | QMessageBox::No
        );

    if (rep != QMessageBox::Yes)
        return;

    if (appliquerModifsDepuisUI()) {
        accept();
    }
}

void modifierproduit::on_Annuler_clicked()
{
    reject();
}
