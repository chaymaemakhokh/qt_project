#include "mainwindow.h"
#include "./domain/enums.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qRegisterMetaType<TypeProduit>("TypeProduit");
    qRegisterMetaType<TypeConteneur>("TypeConteneur");
    qRegisterMetaType<EtatProduit>("EtatProduit");

    MainWindow w;
    w.show();
    return app.exec();
}
