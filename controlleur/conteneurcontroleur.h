#ifndef CONTENEURCONTROLEUR_H
#define CONTENEURCONTROLEUR_H

#include <QObject>
#include <QVector>
#include <memory>

#include "./domain/container.h"
#include "./domain/enums.h"
#include "./domain/product.h"

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

    void debugPrintConteneurs() const;

private:
    Conteneur* trouverConteneurParId(const QString &id) const;

    QVector<std::shared_ptr<Conteneur>> m_conteneurs;
};

#endif // CONTENEURCONTROLEUR_H
