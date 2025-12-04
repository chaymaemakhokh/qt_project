#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // À l’exécution, "Tous" est la valeur par défaut :
    // on force la mise à jour de l’affichage des groupBox
    on_comboBoxTypeModel_currentTextChanged(ui->comboBoxTypeModel->currentText());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBoxTypeModel_currentTextChanged(const QString &text)
{
    // Par sécurité, on part de tout caché
    bool showConteneur = false;
    bool showPalette   = false;
    bool showProduit = false;

    if (text == "Conteneur") {
        showConteneur = true;
    } else if (text == "Palette") {
        showPalette = true;
    }
    else if (text == "Produit") {
        showProduit = true;
    }

    ui->groupBox_Conteneur->setVisible(showConteneur);
    ui->groupBox_Palette->setVisible(showPalette);
    ui->groupBox_Produit->setVisible(showProduit);
}
