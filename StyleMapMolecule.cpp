#include "StyleMapMolecule.h"

StyleMapMolecule::StyleMapMolecule() {}

void StyleMapMolecule::applyTo(MapMolecule *pMap) const
{
    assert(pMap);
    pMap->copyParameters(*this);
    pMap->setGlyphsToUpdate();
}
