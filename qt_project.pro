TEMPLATE = app
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ajouterconteneur.cpp \
    ajouterpalette.cpp \
    ajouterproduit.cpp \
    controlleur/conteneurcontroleur.cpp \
    controlleur/palettecontroleur.cpp \
    controlleur/produitcontroleur.cpp \
    domain/compatibility.cpp \
    domain/container.cpp \
    domain/elements_palette.cpp \
    domain/enums.cpp \
    domain/palette.cpp \
    domain/product.cpp \
    domain/produit_caracteristiques.cpp \
    domain/produit_cycledevie.cpp \
    domain/warehouse.cpp \
    main.cpp \
    mainwindow.cpp \
    model/warehouseitem.cpp \
    model/warehousemodel.cpp \
    modifierconteneur.cpp \
    modifierproduit.cpp

HEADERS += \
    ajouterconteneur.h \
    ajouterpalette.h \
    ajouterproduit.h \
    controlleur/conteneurcontroleur.h \
    controlleur/palettecontroleur.h \
    controlleur/produitcontroleur.h \
    domain/compatibility.h \
    domain/container.h \
    domain/elements_palette.h \
    domain/enums.h \
    domain/palette.h \
    domain/product.h \
    domain/produit_caracteristiques.h \
    domain/produit_cycledevie.h \
    domain/warehouse.h \
    mainwindow.h \
    model/warehouseitem.h \
    model/warehousemodel.h \
    modifierconteneur.h \
    modifierproduit.h

FORMS += \
    ajouterconteneur.ui \
    ajouterpalette.ui \
    ajouterproduit.ui \
    mainwindow.ui \
    modifierconteneur.ui \
    modifierproduit.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
