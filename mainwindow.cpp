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
    bool showConteneur = false;
    bool showPalette   = false;
    bool showProduit   = false;

    if (text == "Conteneur") {
        showConteneur = true;
    } else if (text == "Palette") {
        showPalette = true;
    } else if (text == "Produit") {
        showProduit = true;
    }

    ui->groupBox_Conteneur->setVisible(showConteneur);
    ui->groupBox_Palette->setVisible(showPalette);
    ui->groupBox_Produit->setVisible(showProduit);
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

