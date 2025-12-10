#include "palettecontroleur.h"
#include <QDebug>
#include <QtGlobal>

PaletteControleur::PaletteControleur(QObject *parent)
    : QObject(parent)
{
}

void PaletteControleur::ajouterPalette(const QString &id,
                                       const QString &destination,
                                       const QDate  &dateEnvoiPrevue,
                                       double capaciteMax)
{
    auto p = std::make_shared<Palette>();
    p->setIdPalette(id);
    p->setDestination(destination);
    p->setDateEnvoiPrevue(dateEnvoiPrevue);
    p->setCapaciteMax(capaciteMax);

    m_palettes.push_back(p);
}

QVector<std::shared_ptr<Palette>>
PaletteControleur::rechercherParIdPartiel(const QString &idPartiel) const
{
    QVector<std::shared_ptr<Palette>> res;
    const QString pattern = idPartiel.trimmed();

    for (const auto &p : m_palettes) {
        if (!p) continue;
        if (pattern.isEmpty() ||
            p->idPalette().contains(pattern, Qt::CaseInsensitive)) {
            res.push_back(p);
        }
    }
    return res;
}

QVector<std::shared_ptr<Palette>>
PaletteControleur::rechercherParFiltre(const FiltrePalette &f) const
{
    QVector<std::shared_ptr<Palette>> res;
    const QString idPattern  = f.idPartiel.trimmed();
    const QString destPattern = f.destinationPartielle.trimmed();

    for (const auto &p : m_palettes) {
        if (!p) continue;

        bool ok = true;

        // Id
        if (!idPattern.isEmpty() &&
            !p->idPalette().contains(idPattern, Qt::CaseInsensitive)) {
            ok = false;
        }

        // Destination
        if (ok && !destPattern.isEmpty() &&
            !p->destination().contains(destPattern, Qt::CaseInsensitive)) {
            ok = false;
        }

        // Date d'envoi
        if (ok && f.filtrerParDateEnvoi &&
            p->dateEnvoiPrevue() != f.dateEnvoi) {
            ok = false;
        }

        // Capacité max
        if (ok && f.filtrerParCapacite &&
            !qFuzzyCompare(p->capaciteMax() + 1.0, f.capaciteMax + 1.0)) {
            ok = false;
        }

        if (ok)
            res.push_back(p);
    }

    return res;
}

bool PaletteControleur::supprimerPaletteParId(const QString &id)
{
    for (int i = 0; i < m_palettes.size(); ++i) {
        const auto &p = m_palettes[i];
        if (p && p->idPalette() == id) {
            m_palettes.removeAt(i);
            return true;
        }
    }
    return false;
}

void PaletteControleur::debugPrintPalettes() const
{
    qDebug().noquote() << "===== LISTE DES PALETTES =====";

    for (const auto &p : m_palettes) {
        if (!p) continue;

        QString ligne = QStringLiteral(
                            "ID=%1 | Dest=%2 | DateEnvoi=%3 | CapMax=%4 | NbElements=%5")
                            .arg(p->idPalette())
                            .arg(p->destination())
                            .arg(p->dateEnvoiPrevue().toString(Qt::ISODate))
                            .arg(p->capaciteMax())
                            .arg(p->elements().size());

        qDebug().noquote() << ligne;
    }

    qDebug().noquote() << "================================";
}
