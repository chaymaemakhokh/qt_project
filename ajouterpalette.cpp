#include "ajouterpalette.h"
#include "ui_ajouterpalette.h"

#include "./controlleur/palettecontroleur.h"

ajouterpalette::ajouterpalette(PaletteControleur *ctrl, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ajouterpalette)
    , m_ctrl(ctrl)
{
    ui->setupUi(this);
}

ajouterpalette::~ajouterpalette()
{
    delete ui;
}

void ajouterpalette::on_Creer_clicked()
{
    if (!m_ctrl) {
        accept();
        return;
    }

    QString id          = ui->IdPalette_2->text();          // QLineEdit
    QString destination = ui->Destination_2->text();        // QLineEdit
    QDate dateEnvoi     = ui->DateEnvoiPrevue_2->date();    // QDateEdit
    double capacite     = ui->CapaciteMax_2->value();       // QDoubleSpinBox

    m_ctrl->ajouterPalette(id, destination, dateEnvoi, capacite);

    accept();   // ferme le dialog avec Accepted
}

void ajouterpalette::on_Annuler_clicked()
{
    reject();
}
