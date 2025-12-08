#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ajouterproduit.h"
#include "ajouterconteneur.h"

#include "./controlleur/produitcontroleur.h"
#include "./controlleur/conteneurcontroleur.h"

#include "./domain/container.h"
#include "./domain/product.h"
#include "./domain/enums.h"


#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_produitCtrl(new ProduitControleur(this))
    , m_conteneurCtrl(new ConteneurControleur(this))
    , m_treeModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
    // Quand on tape un Id, on met à jour les détails
    connect(ui->lineEdit, &QLineEdit::textChanged,
            this, &MainWindow::on_lineEditId_textChanged);


    ui->treeView->setModel(m_treeModel);
    ui->treeView->setHeaderHidden(false);

    on_comboBoxTypeModel_currentTextChanged(ui->comboBoxTypeModel->currentText());

    rebuildTreeView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBoxTypeModel_currentTextChanged(const QString &text)
{
    bool showConteneur = (text == "Conteneur");
    bool showPalette   = (text == "Palette");
    bool showProduit   = (text == "Produit");

    ui->groupBox_Conteneur->setVisible(showConteneur);
    ui->groupBox_Palette->setVisible(showPalette);
    ui->groupBox_Produit->setVisible(showProduit);

    // En mode "Tous", on nettoie l’ID et les champs
    if (text == "Tous") {
        ui->lineEdit->clear();
    }

    // Met à jour les champs de détail en fonction du nouveau mode
    updateDetailsFromFilter();
}
void MainWindow::on_lineEditId_textChanged(const QString &)
{
    updateDetailsFromFilter();
}


void MainWindow::rebuildTreeView()
{
    m_treeModel->clear();
    m_treeModel->setHorizontalHeaderLabels(QStringList() << "Entrepôt");

    if (!m_conteneurCtrl)
        return;

    const auto &conts = m_conteneurCtrl->conteneurs();  // nécessite la méthode conteneurs()

    for (const auto &cPtr : conts) {
        if (!cPtr)
            continue;

        // Noeud racine = un conteneur
        QString contLabel = QString("Conteneur %1").arg(cPtr->idConteneur());
        QStandardItem *contItem = new QStandardItem(contLabel);

        // On peut aussi stocker l'id brut si besoin plus tard :
        contItem->setData(cPtr->idConteneur(), Qt::UserRole + 1);

        // Enfants = produits de ce conteneur
        const auto &prods = cPtr->produits();
        for (Product *p : prods) {
            if (!p)
                continue;

            QString prodLabel = QString("Produit %1").arg(p->idProduit());
            QStandardItem *prodItem = new QStandardItem(prodLabel);
            prodItem->setData(p->idProduit(), Qt::UserRole + 1);

            contItem->appendRow(prodItem);
        }

        m_treeModel->appendRow(contItem);
    }

    ui->treeView->expandAll();
}

void MainWindow::on_actionAjoute_Produit_triggered()
{
    ajouterProduit dlg(m_produitCtrl, m_conteneurCtrl, this);
    if (dlg.exec() == QDialog::Accepted) {
        m_produitCtrl->debugPrintProduits();
        m_conteneurCtrl->debugPrintConteneurs();  // pour vérifier que le produit est bien dedans
        rebuildTreeView();
    }
}

void MainWindow::on_actionAjouter_Conteneur_triggered()
{
    ajouterconteneur dlg(m_conteneurCtrl, this);
    if (dlg.exec() == QDialog::Accepted) {
        m_conteneurCtrl->debugPrintConteneurs();
        rebuildTreeView();
    }
}

void MainWindow::updateDetailsFromFilter()
{
    QString mode = ui->comboBoxTypeModel->currentText();  // "Tous", "Conteneur", "Produit"
    QString id   = ui->lineEdit->text().trimmed();

    // D’abord on vide tout
    // ----- conteneur -----
    ui->lineEdit_18->clear();
    ui->comboBox_2->setCurrentIndex(-1);
    ui->lineEdit_2->clear();

    // ----- produit -----
    ui->lineEdit_17->clear();
    ui->comboBox_9->setCurrentIndex(-1);
    ui->lineEdit_13->clear();
    ui->dateEdit_5->setDate(QDate::fromString("01/01/2000","dd/MM/yyyy"));
    ui->lineEdit_14->clear();
    ui->lineEdit_15->clear();
    ui->lineEdit_16->clear();

    if (mode == "Conteneur") {
        if (id.isEmpty() || !m_conteneurCtrl)
            return;

        Conteneur *c = m_conteneurCtrl->getConteneurParId(id);
        if (!c)
            return;

        ui->lineEdit_18->setText(c->idConteneur());
        ui->lineEdit_2->setText(QString::number(c->capaciteMax()));

        // sélectionner le bon type dans la combo
        QString typeStr = toString(c->type());   // ta fonction utilitaire enums.h
        int idx = ui->comboBox_2->findText(typeStr);
        if (idx >= 0)
            ui->comboBox_2->setCurrentIndex(idx);
    }
    else if (mode == "Produit") {
        if (id.isEmpty() || !m_produitCtrl)
            return;

        Product *p = m_produitCtrl->getProduitParId(id);
        if (!p)
            return;

        ui->lineEdit_17->setText(p->idProduit());
        ui->lineEdit_13->setText(p->nom());
        ui->lineEdit_14->setText(QString::number(p->capaciteMax()));
        ui->lineEdit_15->setText(QString::number(p->poids()));
        ui->lineEdit_16->setText(QString::number(p->volume()));
        ui->dateEdit_5->setDate(p->dateEntreeStock());

        QString typeStr = toString(p->type());
        int idx = ui->comboBox_9->findText(typeStr);
        if (idx >= 0)
            ui->comboBox_9->setCurrentIndex(idx);
    }
    // mode "Tous" → on laisse tout vide
}


