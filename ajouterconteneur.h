#ifndef AJOUTERCONTENEUR_H
#define AJOUTERCONTENEUR_H

#include <QDialog>
#include "./domain/enums.h"

class ConteneurControleur;

namespace Ui {
class ajouterconteneur;
}

class ajouterconteneur : public QDialog
{
    Q_OBJECT

public:
    explicit ajouterconteneur(ConteneurControleur *ctrl, QWidget *parent = nullptr);
    ~ajouterconteneur();

private slots:
    void on_Creer_clicked();
    void on_Annuler_clicked();

private:
    Ui::ajouterconteneur *ui;
    ConteneurControleur *m_ctrl;
};

#endif // AJOUTERCONTENEUR_H
