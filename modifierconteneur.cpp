#include "modifierconteneur.h"
#include "ui_modifierconteneur.h"

#include "./controlleur/conteneurcontroleur.h"
#include "./domain/container.h"
#include "./domain/enums.h"

#include <QVariant>
#include <QMessageBox>

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

modifierconteneur::modifierconteneur(const QString &idConteneur,
                                     ConteneurControleur *contCtrl,
                                     QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::modifierconteneur)
    , m_idConteneur(idConteneur.trimmed())
    , m_contCtrl(contCtrl)
{
    ui->setupUi(this);

    // Id affiché mais non modifiable
    ui->IdConteneur_2->setReadOnly(true);
    ui->IdConteneur_2->setEnabled(false);

    remplirComboType();
    chargerConteneurDansUI();
}

modifierconteneur::~modifierconteneur()
{
    delete ui;
}

void modifierconteneur::remplirComboType()
{
    ui->TypeConteneur_2->clear();
    for (TypeConteneur t : {TypeConteneur::Normal,
                            TypeConteneur::Froid,
                            TypeConteneur::Fragile,
                            TypeConteneur::Autre}) {
        ui->TypeConteneur_2->addItem(toString(t), QVariant::fromValue(t));
    }
}

void modifierconteneur::chargerConteneurDansUI()
{
    if (!m_contCtrl || m_idConteneur.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Contrôleur conteneur invalide."));
        reject();
        return;
    }

    Conteneur *c = m_contCtrl->trouverConteneurParId(m_idConteneur);
    if (!c) {
        QMessageBox::warning(this, tr("Erreur"),
                             tr("Conteneur introuvable (id=%1).").arg(m_idConteneur));
        reject();
        return;
    }

    ui->IdConteneur_2->setText(c->idConteneur());
    setComboByData(ui->TypeConteneur_2, QVariant::fromValue(c->type()));
    ui->CapaciteMax_2->setValue(c->capaciteMax());
}

bool modifierconteneur::appliquerModifsDepuisUI()
{
    if (!m_contCtrl || m_idConteneur.isEmpty()) return false;

    Conteneur *c = m_contCtrl->trouverConteneurParId(m_idConteneur);
    if (!c) return false;

    const TypeConteneur newType = ui->TypeConteneur_2->currentData().value<TypeConteneur>();
    const double newCap = ui->CapaciteMax_2->value();

    // ✅ règle importante : on ne peut pas réduire la capacité sous ce qui est déjà utilisé
    const double used = c->capaciteUtilisee();
    if (newCap + 1e-9 < used) {
        QMessageBox::warning(
            this,
            tr("Modification impossible"),
            tr("Capacité trop petite.\n"
               "Capacité utilisée par les produits = %1\n"
               "Capacité demandée = %2")
                .arg(used)
                .arg(newCap)
            );
        return false;
    }

    c->setType(newType);
    c->setCapaciteMax(newCap);

    return true;
}

void modifierconteneur::on_Modifier_clicked()
{
    const auto rep = QMessageBox::question(
        this,
        tr("Confirmer la modification"),
        tr("Vous êtes sûr(e) de vouloir modifier ce conteneur ?"),
        QMessageBox::Yes | QMessageBox::No
        );

    if (rep != QMessageBox::Yes) return;

    if (appliquerModifsDepuisUI())
        accept();
}

void modifierconteneur::on_Annuler_clicked()
{
    reject();
}
