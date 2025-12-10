#ifndef AJOUTERPALETTE_H
#define AJOUTERPALETTE_H

#include <QDialog>
#include <QDate>

class PaletteControleur;

namespace Ui {
class ajouterpalette;
}

class ajouterpalette : public QDialog
{
    Q_OBJECT

public:
    explicit ajouterpalette(PaletteControleur *ctrl, QWidget *parent = nullptr);
    ~ajouterpalette();

private slots:
    void on_Creer_clicked();
    void on_Annuler_clicked();

private:
    Ui::ajouterpalette *ui;
    PaletteControleur  *m_ctrl;
};

#endif // AJOUTERPALETTE_H
