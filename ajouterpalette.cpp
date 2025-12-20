#include "ajouterpalette.h"
#include "ui_ajouterpalette.h"

#include "./controlleur/palettecontroleur.h"
#include "./controlleur/produitcontroleur.h"

#include "./domain/product.h"
#include "./domain/enums.h"

#include <QVariant>
#include <QHeaderView>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QSignalBlocker>
#include <QMessageBox>

static void setLastSectionStretch(QTableWidget *t)
{
    if (!t || !t->horizontalHeader()) return;
    t->horizontalHeader()->setStretchLastSection(true);
}

ajouterpalette::ajouterpalette(PaletteControleur *palCtrl,
                               ProduitControleur *prodCtrl,
                               QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ajouterpalette)
    , m_palCtrl(palCtrl)
    , m_prodCtrl(prodCtrl)
{
    ui->setupUi(this);

    // ✅ Toggle auto-id => désactive IdPalette
    on_checkBox_IdAutomatique_toggled(ui->checkBox_IdAutomatique->isChecked());

    // (optionnel) date par défaut
    if (ui->DateEnvoiPrevue_2)
        ui->DateEnvoiPrevue_2->setDate(QDate::currentDate());

    remplirComboTypes();
    initialiserTable();

    connect(ui->comboBox_TypeProduits,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &ajouterpalette::on_comboBox_TypeProduits_currentIndexChanged);

    // ✅ première construction : TOUS
    rebuildTableProduits();
}

ajouterpalette::~ajouterpalette()
{
    delete ui;
}

void ajouterpalette::initialiserTable()
{
    // Colonnes (adapte si tu veux plus/moins)
    ui->tableProduits->setColumnCount(9);
    ui->tableProduits->setHorizontalHeaderLabels(QStringList()
                                                 << "Id" << "Nom" << "Type"
                                                 << "Capacité" << "Poids" << "Volume"
                                                 << "État" << "Date entrée"
                                                 << "Choisir");

    // ✅ responsive : contenu lisible + last column stretch
    ui->tableProduits->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setLastSectionStretch(ui->tableProduits);

    ui->tableProduits->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableProduits->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableProduits->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void ajouterpalette::remplirComboTypes()
{
    ui->comboBox_TypeProduits->clear();

    // TOUS => data = -1
    ui->comboBox_TypeProduits->addItem("TOUS", -1);

    // Types réels
    for (TypeProduit t : {TypeProduit::Alimentaire,
                          TypeProduit::Electronique,
                          TypeProduit::Medicament,
                          TypeProduit::Autre})
    {
        ui->comboBox_TypeProduits->addItem(toString(t), QVariant::fromValue((int)t));
    }

    ui->comboBox_TypeProduits->setCurrentIndex(0); // TOUS
}

void ajouterpalette::on_comboBox_TypeProduits_currentIndexChanged(int)
{
    rebuildTableProduits();
}

void ajouterpalette::rebuildTableProduits()
{
    if (!m_prodCtrl) return;

    const QSignalBlocker blocker(ui->tableProduits); // évite des signaux parasites

    ui->tableProduits->setRowCount(0);

    const int v = ui->comboBox_TypeProduits->currentData().toInt();
    const bool tous = (v == -1);

    QVector<std::shared_ptr<Product>> list;
    if (tous) {
        list = m_prodCtrl->produits();
    } else {
        list = m_prodCtrl->produitsParType((TypeProduit)v);
    }

    for (const auto &p : list) {
        ajouterLigneProduit(p);
    }

    // ✅ garde un affichage propre
    ui->tableProduits->resizeColumnsToContents();
    setLastSectionStretch(ui->tableProduits);
}

void ajouterpalette::ajouterLigneProduit(const std::shared_ptr<Product> &p)
{
    if (!p) return;

    const int row = ui->tableProduits->rowCount();
    ui->tableProduits->insertRow(row);

    auto mk = [&](const QString &s) {
        auto *it = new QTableWidgetItem(s);
        it->setFlags(it->flags() & ~Qt::ItemIsEditable);
        return it;
    };

    const QString pid = p->idProduit().trimmed();

    ui->tableProduits->setItem(row, 0, mk(pid));
    ui->tableProduits->setItem(row, 1, mk(p->nom()));
    ui->tableProduits->setItem(row, 2, mk(toString(p->type())));
    ui->tableProduits->setItem(row, 3, mk(QString::number(p->capaciteMax())));
    ui->tableProduits->setItem(row, 4, mk(QString::number(p->poids())));
    ui->tableProduits->setItem(row, 5, mk(QString::number(p->volume())));
    ui->tableProduits->setItem(row, 6, mk(toString(p->etat())));
    ui->tableProduits->setItem(row, 7, mk(p->dateEntreeStock().toString("dd/MM/yyyy")));

    // ✅ Checkbox persistante
    auto *cb = new QCheckBox();
    cb->setChecked(m_produitsSelectionnes.contains(pid));

    connect(cb, &QCheckBox::toggled, this, [this, pid](bool checked) {
        if (pid.isEmpty()) return;
        if (checked) m_produitsSelectionnes.insert(pid);
        else         m_produitsSelectionnes.remove(pid);
    });

    QWidget *w = new QWidget();
    auto *lay = new QHBoxLayout(w);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setAlignment(Qt::AlignCenter);
    lay->addWidget(cb);

    ui->tableProduits->setCellWidget(row, 8, w);
}

void ajouterpalette::on_checkBox_IdAutomatique_toggled(bool checked)
{
    // ✅ désactiver / griser le champ id
    ui->IdPalette_2->setEnabled(!checked);
    ui->IdPalette_2->setReadOnly(checked);

    if (checked)
        ui->IdPalette_2->clear();
}

void ajouterpalette::on_Creer_clicked()
{
    // Tu m’as dit que tu laisses la logique de création après.
    // Ici tu as déjà m_produitsSelectionnes qui contient les IDs choisis.
    accept();
}

void ajouterpalette::on_Annuler_clicked()
{
    reject();
}
