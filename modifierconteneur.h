#ifndef MODIFIERCONTENEUR_H
#define MODIFIERCONTENEUR_H

#include <QDialog>
#include "./domain/enums.h"

class ConteneurControleur;

namespace Ui {
class modifierconteneur;
}

class modifierconteneur : public QDialog
{
    Q_OBJECT

public:
    explicit modifierconteneur(const QString &idConteneur,
                               ConteneurControleur *contCtrl,
                               QWidget *parent = nullptr);
    ~modifierconteneur();

private slots:
    void on_Modifier_clicked();
    void on_Annuler_clicked();

private:
    void remplirComboType();
    void chargerConteneurDansUI();
    bool appliquerModifsDepuisUI();

private:
    Ui::modifierconteneur *ui;
    QString m_idConteneur;
    ConteneurControleur *m_contCtrl {nullptr};
};

#endif // MODIFIERCONTENEUR_H
