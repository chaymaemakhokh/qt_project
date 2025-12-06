#include "ajouterconteneur.h"
#include "ui_ajouterconteneur.h"

#include "./controlleur/conteneurcontroleur.h"
#include "./domain/enums.h"
#include <QVariant>

ajouterconteneur::ajouterconteneur(ConteneurControleur *ctrl, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ajouterconteneur)
    , m_ctrl(ctrl)
{
    ui->setupUi(this);

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
    if (!m_ctrl) {
        accept();
        return;
    }

    QString id = ui->IdConteneur_2->text();          // QLineEdit
    double capacite = ui->CapaciteMax_2->value();    // QDoubleSpinBox
    TypeConteneur type =
        ui->TypeConteneur_2->currentData().value<TypeConteneur>();

    m_ctrl->ajouterConteneur(id, type, capacite);

    accept();   // ferme le dialog avec Accepted
}

void ajouterconteneur::on_Annuler_clicked()
{
    reject();   // ferme avec Rejected
}
