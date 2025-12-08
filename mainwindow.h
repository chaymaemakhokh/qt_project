#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./controlleur/produitcontroleur.h"
#include "./controlleur/conteneurcontroleur.h"

class QStandardItemModel;

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
    void on_lineEditId_textChanged(const QString &text);

private:
    void rebuildTreeView();
    void updateDetailsFromFilter();
    Ui::MainWindow *ui;

    ProduitControleur   *m_produitCtrl;
    ConteneurControleur *m_conteneurCtrl;

    QStandardItemModel  *m_treeModel;

};

#endif // MAINWINDOW_H
