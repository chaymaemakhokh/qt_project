#include "ajouterconteneur.h"
#include "ui_ajouterconteneur.h"

#include "./controlleur/conteneurcontroleur.h"
#include "./domain/enums.h"
#include <QVariant>

#include <QMessageBox>

ajouterconteneur::ajouterconteneur(ConteneurControleur *ctrl, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ajouterconteneur)
    , m_ctrl(ctrl)
{
    ui->setupUi(this);
    if (ui->checkBox_Id_Automatique) {
        connect(ui->checkBox_Id_Automatique, &QCheckBox::toggled, this, [this](bool checked){
            ui->IdConteneur_2->setEnabled(!checked);
            if (checked) ui->IdConteneur_2->clear();
        });
    }

    ui->TypeConteneur_2->clear();
    for (TypeConteneur t : {TypeConteneur::Normal,
                            TypeConteneur::Froid,
                            TypeConteneur::Fragile,
                            TypeConteneur::Autre})
    {
        ui->TypeConteneur_2->addItem(toString(t), QVariant::fromValue(t));
    }
}

ajouterconteneur::~ajouterconteneur()
{
    delete ui;
}

void ajouterconteneur::on_Creer_clicked()
{
    if (!m_ctrl) { accept(); return; }

    // ✅ ID auto / manuel
    const bool idAuto =
        (ui->checkBox_Id_Automatique && ui->checkBox_Id_Automatique->isChecked());

    QString id;

    if (idAuto) {
        id = m_ctrl->genererIdConteneurUnique(8); // C + 8 caractères
        ui->IdConteneur_2->setText(id);
    } else {
        id = ui->IdConteneur_2->text().trimmed();

        if (id.isEmpty()) {
            QMessageBox::warning(this, tr("ID invalide"),
                                 tr("Veuillez saisir un ID conteneur."));
            return;
        }

        if (m_ctrl->idExiste(id)) {
            QMessageBox::warning(this, tr("ID déjà utilisé"),
                                 tr("L'ID \"%1\" existe déjà.\nVeuillez en choisir un autre.")
                                     .arg(id));
            ui->IdConteneur_2->setFocus();
            ui->IdConteneur_2->selectAll();
            return;
        }
    }

    // champs restants
    double capacite = ui->CapaciteMax_2->value();
    TypeConteneur type = ui->TypeConteneur_2->currentData().value<TypeConteneur>();

    m_ctrl->ajouterConteneur(id, type, capacite);

    accept();
}

void ajouterconteneur::on_Annuler_clicked()
{
    reject();   // ferme avec Rejected
}
