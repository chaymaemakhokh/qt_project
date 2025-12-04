#include "elements_palette.h"
#include "product.h"

double ElementsPalette::poidsTotal() const
{
    double total = 0.0;
    for (Product *p : m_produits)
    {
        if (p)
            total += p->poids();  // polymorphisme
    }
    return total * m_quantite;
}
