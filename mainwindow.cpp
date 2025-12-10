#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ajouterproduit.h"
#include "ajouterconteneur.h"
#include "ajouterpalette.h"

#include "./controlleur/produitcontroleur.h"
#include "./controlleur/conteneurcontroleur.h"
#include "./controlleur/palettecontroleur.h"

#include "./domain/container.h"
#include "./domain/product.h"
#include "./domain/palette.h"
#include "./domain/enums.h"

#include <QStandardItemModel>
#include <QDialog>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QDialogButtonBox>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_produitCtrl(new ProduitControleur(this))
    , m_conteneurCtrl(new ConteneurControleur(this))
    , m_paletteCtrl(new PaletteControleur(this))
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

    // 1) Conteneurs + produits
    if (m_conteneurCtrl) {
        const auto &conts = m_conteneurCtrl->conteneurs();

        for (const auto &cPtr : conts) {
            if (!cPtr)
                continue;

            QString contLabel = QString("Conteneur %1").arg(cPtr->idConteneur());
            QStandardItem *contItem = new QStandardItem(contLabel);
            contItem->setData(cPtr->idConteneur(), Qt::UserRole + 1);

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
    }

    // 2) Palettes (au même niveau que les conteneurs, après eux)
    if (m_paletteCtrl) {
        const auto &pals = m_paletteCtrl->palettes();

        for (const auto &pPtr : pals) {
            if (!pPtr)
                continue;

            QString palLabel = QString("Palette %1").arg(pPtr->idPalette());
            QStandardItem *palItem = new QStandardItem(palLabel);
            palItem->setData(pPtr->idPalette(), Qt::UserRole + 1);

            // Pour l'instant, on n'affiche pas encore les éléments de palette
            // (on les ajoutera plus tard).
            m_treeModel->appendRow(palItem);
        }
    }

    ui->treeView->expandAll();
}

void MainWindow::on_actionAjoute_Produit_triggered()
{
    ajouterProduit dlg(m_produitCtrl, m_conteneurCtrl, this);
    if (dlg.exec() == QDialog::Accepted) {
        m_produitCtrl->debugPrintProduits();
        m_conteneurCtrl->debugPrintConteneurs();
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

void MainWindow::on_actionAjouter_Palette_triggered()
{
    ajouterpalette dlg(m_paletteCtrl, this);
    if (dlg.exec() == QDialog::Accepted) {
        m_paletteCtrl->debugPrintPalettes();
        rebuildTreeView();
    }
}

void MainWindow::on_Rechercher_clicked()
{
    const QString mode = ui->comboBoxTypeModel->currentText();

    if (mode == "Tous") {
        const QString pattern = ui->IdTous_2->text().trimmed();

        QVector<std::shared_ptr<Conteneur>> conts;
        QVector<std::shared_ptr<Product>>   prods;
        QVector<std::shared_ptr<Palette>>   pals;

        if (m_conteneurCtrl)
            conts = m_conteneurCtrl->rechercherParIdPartiel(pattern);
        if (m_produitCtrl)
            prods = m_produitCtrl->rechercherParIdPartiel(pattern);
        if (m_paletteCtrl)
            pals = m_paletteCtrl->rechercherParIdPartiel(pattern);

        afficherResultatsTous(conts, prods, pals);
    }
    else if (mode == "Conteneur") {
        FiltreConteneur f;
        f.idPartiel = ui->IdConteneur_2->text().trimmed();

        // on filtre TOUJOURS par type (plus de "TOUS" dans cette combo)
        f.filtrerParType = true;
        f.type = ui->TypeConteneur_2->currentData().value<TypeConteneur>();

        double cap = ui->CapaciteMaxConteneur_2->value();
        if (cap > 0.0) {
            f.filtrerParCapacite = true;
            f.capaciteMax = cap;
        }

        auto conts = m_conteneurCtrl->rechercherParFiltre(f);
        afficherListeConteneurs(conts);
    }
    else if (mode == "Produit") {
        FiltreProduit f;

        // Id
        f.idPartiel = ui->IdProduit_2->text().trimmed();

        // TypeProduit : "TOUS" => pas de filtre
        QString typeText = ui->TypeProduit_2->currentText();
        if (typeText != "TOUS") {
            f.filtrerParType = true;
            f.type = typeProduitFromString(typeText);
        }

        // Nom
        f.nomPartiel = ui->NomProduit_2->text().trimmed();

        // DateEntreeStock : 01/01/2000 => pas de filtre
        QDate defaultDate(2000, 1, 1);
        QDate dEntree = ui->DateEntreeStockProduit_2->date();
        if (dEntree != defaultDate) {
            f.filtrerParDateEntree = true;
            f.dateEntree = dEntree;
        }

        // Capacité max (QLineEdit)
        bool ok = false;
        double cap = ui->CapaciteMaxProduit_2->text().trimmed().toDouble(&ok);
        if (ok) {
            f.filtrerParCapacite = true;
            f.capaciteMax = cap;
        }

        // Poids
        ok = false;
        double poids = ui->PoidsProduit_2->text().trimmed().toDouble(&ok);
        if (ok) {
            f.filtrerParPoids = true;
            f.poids = poids;
        }

        // Volume
        ok = false;
        double vol = ui->VolumeProduit_2->text().trimmed().toDouble(&ok);
        if (ok) {
            f.filtrerParVolume = true;
            f.volume = vol;
        }

        auto prods = m_produitCtrl->rechercherParFiltre(f);
        afficherListeProduits(prods);
    }
    else if (mode == "Palette") {
        FiltrePalette f;

        // Id
        f.idPartiel = ui->IdPalette_2->text().trimmed();

        // Destination
        f.destinationPartielle = ui->DestinationPalette_2->text().trimmed();

        // DateEnvoiPrevue : 01/01/2000 => pas de filtre
        QDate defaultDate(2000, 1, 1);
        QDate dEnvoi = ui->DateEnvoiPrevuePalette_2->date();
        if (dEnvoi != defaultDate) {
            f.filtrerParDateEnvoi = true;
            f.dateEnvoi = dEnvoi;
        }

        // Capacité max
        bool ok = false;
        double cap = ui->CapaciteMaxPalette_2->text().trimmed().toDouble(&ok);
        if (ok) {
            f.filtrerParCapacite = true;
            f.capaciteMax = cap;
        }

        auto pals = m_paletteCtrl->rechercherParFiltre(f);
        afficherListePalettes(pals);
    }
    else {
        QMessageBox::information(
            this,
            tr("Filtre"),
            tr("Type de filtre non géré."));
    }
}

void MainWindow::afficherResultatsTous(
    const QVector<std::shared_ptr<Conteneur>> &conts,
    const QVector<std::shared_ptr<Product>>   &prods,
    const QVector<std::shared_ptr<Palette>>   &pals)
{
    if (conts.isEmpty() && prods.isEmpty() && pals.isEmpty()) {
        QMessageBox::information(this, tr("Résultats"),
                                 tr("Aucun résultat trouvé."));
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle(tr("Résultats de la recherche"));
    QVBoxLayout *layout = new QVBoxLayout(&dlg);

    QTabWidget *tabs = new QTabWidget(&dlg);
    layout->addWidget(tabs);

    // --- Conteneurs ---
    if (!conts.isEmpty()) {
        QTableWidget *table = new QTableWidget(conts.size(), 3, &dlg);
        table->setHorizontalHeaderLabels(
            QStringList() << "Id" << "Type" << "CapaciteMax");
        table->horizontalHeader()->setStretchLastSection(true);

        for (int i = 0; i < conts.size(); ++i) {
            const auto &c = conts[i];
            table->setItem(i, 0, new QTableWidgetItem(c->idConteneur()));
            table->setItem(i, 1, new QTableWidgetItem(toString(c->type())));
            table->setItem(i, 2, new QTableWidgetItem(QString::number(c->capaciteMax())));

            // TODO plus tard : ajouter 2 colonnes avec des boutons d'action
        }

        tabs->addTab(table, tr("Conteneurs"));
    }

    // --- Produits ---
    if (!prods.isEmpty()) {
        QTableWidget *table = new QTableWidget(prods.size(), 3, &dlg);
        table->setHorizontalHeaderLabels(
            QStringList() << "Id" << "Nom" << "Type");
        table->horizontalHeader()->setStretchLastSection(true);

        for (int i = 0; i < prods.size(); ++i) {
            const auto &p = prods[i];
            table->setItem(i, 0, new QTableWidgetItem(p->idProduit()));
            table->setItem(i, 1, new QTableWidgetItem(p->nom()));
            table->setItem(i, 2, new QTableWidgetItem(toString(p->type())));
        }

        tabs->addTab(table, tr("Produits"));
    }

    // --- Palettes ---
    if (!pals.isEmpty()) {
        QTableWidget *table = new QTableWidget(pals.size(), 3, &dlg);
        table->setHorizontalHeaderLabels(
            QStringList() << "Id" << "Destination" << "Date envoi");
        table->horizontalHeader()->setStretchLastSection(true);

        for (int i = 0; i < pals.size(); ++i) {
            const auto &p = pals[i];
            table->setItem(i, 0, new QTableWidgetItem(p->idPalette()));
            table->setItem(i, 1, new QTableWidgetItem(p->destination()));
            table->setItem(i, 2, new QTableWidgetItem(
                                     p->dateEnvoiPrevue().toString(Qt::ISODate)));
        }

        tabs->addTab(table, tr("Palettes"));
    }

    QDialogButtonBox *buttons =
        new QDialogButtonBox(QDialogButtonBox::Close, &dlg);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    dlg.exec();
}

void MainWindow::afficherListeConteneurs(
    const QVector<std::shared_ptr<Conteneur>> &conts)
{
    if (conts.isEmpty()) {
        QMessageBox::information(this, tr("Résultats"),
                                 tr("Aucun conteneur ne correspond au filtre."));
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle(tr("Conteneurs trouvés"));
    QVBoxLayout *layout = new QVBoxLayout(&dlg);

    // 5 colonnes data + 2 actions
    QTableWidget *table = new QTableWidget(conts.size(), 7, &dlg);
    table->setHorizontalHeaderLabels(
        QStringList() << "Id" << "Type" << "CapaciteMax"
                      << "NbProduits" << "PoidsTotal"
                      << "Editer" << "Supprimer");
    table->horizontalHeader()->setStretchLastSection(true);

    for (int i = 0; i < conts.size(); ++i) {
        const auto &c = conts[i];
        if (!c) continue;

        QString id = c->idConteneur();

        table->setItem(i, 0, new QTableWidgetItem(id));
        table->setItem(i, 1, new QTableWidgetItem(toString(c->type())));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(c->capaciteMax())));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(c->produits().size())));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(c->poidsTotal())));

        // Boutons actions
        QPushButton *btnEdit = new QPushButton("✏", table);
        QPushButton *btnDel  = new QPushButton("🗑", table);
        btnEdit->setFlat(true);
        btnDel->setFlat(true);

        table->setCellWidget(i, 5, btnEdit);
        table->setCellWidget(i, 6, btnDel);

        // TODO plus tard : connecter btnEdit pour ouvrir un dialog d'édition

        connect(btnDel, &QPushButton::clicked, this,
                [this, id, table]() {
                    if (!m_conteneurCtrl) return;
                    if (!m_conteneurCtrl->supprimerConteneurParId(id)) return;

                    rebuildTreeView();

                    // enlever la ligne de la table
                    for (int r = 0; r < table->rowCount(); ++r) {
                        QTableWidgetItem *item = table->item(r, 0);
                        if (item && item->text() == id) {
                            table->removeRow(r);
                            break;
                        }
                    }
                });
    }

    layout->addWidget(table);

    QDialogButtonBox *buttons =
        new QDialogButtonBox(QDialogButtonBox::Close, &dlg);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    dlg.exec();
}

void MainWindow::afficherListeProduits(
    const QVector<std::shared_ptr<Product>> &prods)
{
    if (prods.isEmpty()) {
        QMessageBox::information(this, tr("Résultats"),
                                 tr("Aucun produit ne correspond au filtre."));
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle(tr("Produits trouvés"));
    QVBoxLayout *layout = new QVBoxLayout(&dlg);

    QTableWidget *table = new QTableWidget(prods.size(), 6, &dlg);
    table->setHorizontalHeaderLabels(
        QStringList() << "Id" << "Nom" << "Type" << "CapaciteMax"
                      << "Editer" << "Supprimer");
    table->horizontalHeader()->setStretchLastSection(true);

    for (int i = 0; i < prods.size(); ++i) {
        const auto &p = prods[i];
        if (!p) continue;

        QString id = p->idProduit();

        table->setItem(i, 0, new QTableWidgetItem(id));
        table->setItem(i, 1, new QTableWidgetItem(p->nom()));
        table->setItem(i, 2, new QTableWidgetItem(toString(p->type())));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(p->capaciteMax())));

        QPushButton *btnEdit = new QPushButton("✏", table);
        QPushButton *btnDel  = new QPushButton("🗑", table);
        btnEdit->setFlat(true);
        btnDel->setFlat(true);

        table->setCellWidget(i, 4, btnEdit);
        table->setCellWidget(i, 5, btnDel);

        // TODO plus tard : gestion de l'édition

        connect(btnDel, &QPushButton::clicked, this,
                [this, id, table]() {
                    // 1) retirer ce produit de tous les conteneurs
                    if (m_conteneurCtrl) {
                        m_conteneurCtrl->supprimerProduitDansTousLesConteneursParId(id);
                    }

                    // 2) supprimer le produit du contrôleur produit
                    if (!m_produitCtrl || !m_produitCtrl->supprimerProduitParId(id))
                        return;

                    // 3) mettre à jour l'arbre hiérarchique
                    rebuildTreeView();

                    // 4) enlever la ligne de la table
                    for (int r = 0; r < table->rowCount(); ++r) {
                        QTableWidgetItem *item = table->item(r, 0);
                        if (item && item->text() == id) {
                            table->removeRow(r);
                            break;
                        }
                    }
                });
    }

    layout->addWidget(table);

    QDialogButtonBox *buttons =
        new QDialogButtonBox(QDialogButtonBox::Close, &dlg);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    dlg.exec();
}


void MainWindow::afficherListePalettes(
    const QVector<std::shared_ptr<Palette>> &pals)
{
    if (pals.isEmpty()) {
        QMessageBox::information(this, tr("Résultats"),
                                 tr("Aucune palette ne correspond au filtre."));
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle(tr("Palettes trouvées"));
    QVBoxLayout *layout = new QVBoxLayout(&dlg);

    QTableWidget *table = new QTableWidget(pals.size(), 6, &dlg);
    table->setHorizontalHeaderLabels(
        QStringList() << "Id" << "Destination" << "Date envoi" << "CapaciteMax"
                      << "Editer" << "Supprimer");
    table->horizontalHeader()->setStretchLastSection(true);

    for (int i = 0; i < pals.size(); ++i) {
        const auto &p = pals[i];
        if (!p) continue;

        QString id = p->idPalette();

        table->setItem(i, 0, new QTableWidgetItem(id));
        table->setItem(i, 1, new QTableWidgetItem(p->destination()));
        table->setItem(i, 2, new QTableWidgetItem(
                                 p->dateEnvoiPrevue().toString(Qt::ISODate)));
        table->setItem(i, 3, new QTableWidgetItem(
                                 QString::number(p->capaciteMax())));

        QPushButton *btnEdit = new QPushButton("✏", table);
        QPushButton *btnDel  = new QPushButton("🗑", table);
        btnEdit->setFlat(true);
        btnDel->setFlat(true);

        table->setCellWidget(i, 4, btnEdit);
        table->setCellWidget(i, 5, btnDel);

        // TODO : btnEdit plus tard

        connect(btnDel, &QPushButton::clicked, this,
                [this, id, table]() {
                    if (!m_paletteCtrl) return;
                    if (!m_paletteCtrl->supprimerPaletteParId(id)) return;

                    rebuildTreeView();

                    for (int r = 0; r < table->rowCount(); ++r) {
                        QTableWidgetItem *item = table->item(r, 0);
                        if (item && item->text() == id) {
                            table->removeRow(r);
                            break;
                        }
                    }
                });
    }

    layout->addWidget(table);

    QDialogButtonBox *buttons =
        new QDialogButtonBox(QDialogButtonBox::Close, &dlg);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    dlg.exec();
}





