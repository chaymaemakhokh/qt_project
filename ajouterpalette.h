#ifndef AJOUTERPALETTE_H
#define AJOUTERPALETTE_H

#include <QDialog>
#include <QSet>
#include <QString>
#include <memory>

#include "./domain/enums.h"

class Product;
class PaletteControleur;
class ProduitControleur;

namespace Ui { class ajouterpalette; }

class ajouterpalette : public QDialog
{
    Q_OBJECT

public:
    explicit ajouterpalette(PaletteControleur *palCtrl,
                            ProduitControleur *prodCtrl,
                            QWidget *parent = nullptr);
    ~ajouterpalette();

private slots:
    void on_comboBox_TypeProduits_currentIndexChanged(int index);
    void on_checkBox_IdAutomatique_toggled(bool checked);
    void on_Creer_clicked();
    void on_Annuler_clicked();

private:
    void initialiserTable();
    void remplirComboTypes();
    void rebuildTableProduits();                 // <- rebuild en gardant sélection
    void ajouterLigneProduit(const std::shared_ptr<Product> &p);

private:
    Ui::ajouterpalette *ui {nullptr};
    PaletteControleur *m_palCtrl {nullptr};
    ProduitControleur *m_prodCtrl {nullptr};

    // ✅ persistance de sélection, même si on change de type
    QSet<QString> m_produitsSelectionnes;
};

#endif // AJOUTERPALETTE_H
