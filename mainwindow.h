#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <memory>

#include "./controlleur/produitcontroleur.h"
#include "./controlleur/conteneurcontroleur.h"
#include "./controlleur/palettecontroleur.h"
#include "./domain/compatibility.h"

class QStandardItemModel;
class Conteneur;
class Product;
class Palette;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_comboBoxTypeModel_currentTextChanged(const QString &text);

    void on_actionAjoute_Produit_triggered();
    void on_actionAjouter_Conteneur_triggered();
    void on_actionAjouter_Palette_triggered();

    void on_Rechercher_clicked();
    void on_actionExporter_JSON_triggered();
    void on_actionImporter_JSON_triggered();

    void on_actionGenerer_Palettes_Automatiquement_triggered();


private:
    void rebuildTreeView();

    void afficherResultatsTous(
        const QVector<std::shared_ptr<Conteneur>> &conts,
        const QVector<std::shared_ptr<Product>>   &prods,
        const QVector<std::shared_ptr<Palette>>   &pals);

    void afficherListeConteneurs(
        const QVector<std::shared_ptr<Conteneur>> &conts);

    void afficherListeProduits(
        const QVector<std::shared_ptr<Product>> &prods);

    void afficherListePalettes(
        const QVector<std::shared_ptr<Palette>> &pals);

    Ui::MainWindow *ui;

    ProduitControleur   *m_produitCtrl;
    ConteneurControleur *m_conteneurCtrl;
    PaletteControleur   *m_paletteCtrl;

    QStandardItemModel  *m_treeModel;

    ReglesCompatibilite m_reglesCompat;
};

#endif // MAINWINDOW_H
