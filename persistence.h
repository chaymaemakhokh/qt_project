#pragma once

#include <QString>

class ConteneurControleur;
class ProduitControleur;
class PaletteControleur;

namespace Persistence {

bool exporterJSON(const QString &fileName,
                  const ConteneurControleur *contCtrl,
                  const ProduitControleur   *prodCtrl,
                  const PaletteControleur   *palCtrl);

bool importerJSON(const QString &fileName,
                  ConteneurControleur *contCtrl,
                  ProduitControleur   *prodCtrl,
                  PaletteControleur   *palCtrl);

} // namespace Persistence
