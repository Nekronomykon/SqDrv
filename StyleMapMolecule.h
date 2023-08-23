#ifndef Style_MapMolecule_h__
#define Style_MapMolecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "MapMoleculeParams.h"
#include "MapMolecule.h"

class StyleMapMolecule
    : public MapMoleculeParams
{
public:
  explicit StyleMapMolecule();

  void applyTo(MapMolecule * /*pMap*/) const;

  static const MapMoleculeParams constexpr StyleCPKSphere = {false, MapMoleculeParams::VDWRadius, MapMoleculeParams::DiscreteByAtom, 1.0f, 0};
};

#endif // !Style_MapMolecule_h__
