#ifndef CONTENEURCONTROLEUR_H
#define CONTENEURCONTROLEUR_H

#include <QObject>
#include <QVector>
#include <memory>
#include <QString>

#include "./domain/container.h"
#include "./domain/enums.h"
#include "./domain/product.h"


struct FiltreConteneur
{
    QString idPartiel;
    bool filtrerParType = false;
    TypeConteneur type = TypeConteneur::Normal;

    bool filtrerParCapacite = false;
    double capaciteMax = 0.0;
};

class ConteneurControleur : public QObject
{
    Q_OBJECT
public:
    explicit ConteneurControleur(QObject *parent = nullptr);

    void ajouterConteneur(const QString &id,
                          TypeConteneur type,
                          double capaciteMax);

    // accès en lecture aux conteneurs (pour remplir la combo)
    const QVector<std::shared_ptr<Conteneur>>& conteneurs() const { return m_conteneurs; }

    // ajouter un produit dans un conteneur donné par son id
    bool ajouterProduitAuConteneur(const QString &idConteneur,
                                   const std::shared_ptr<Product> &produit);

    QVector<std::shared_ptr<Conteneur>> rechercherParFiltre(const FiltreConteneur &filtre) const;

    bool supprimerConteneurParId(const QString &id);

    bool supprimerProduitDansTousLesConteneursParId(const QString &idProduit);

    Conteneur* trouverConteneurParId(const QString &id) const;

    bool peutAjouterProduitAuConteneur(const QString &idConteneur, double capaciteProduit) const;

    bool peutModifierCapaciteProduit(const QString &idProduit,
                                     double nouvelleCapacite,
                                     QString *conteneurBloquant = nullptr,
                                     double *capaciteRestante = nullptr) const;

    /******/
    void vider();

    bool idExiste(const QString &id) const;
    QString genererIdConteneurUnique(int longueur = 8) const;



    void debugPrintConteneurs() const;

private:
    QVector<std::shared_ptr<Conteneur>> m_conteneurs;
};

#endif // CONTENEURCONTROLEUR_H
