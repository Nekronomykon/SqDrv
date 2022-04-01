#include <vtkMoleculeMapper.h>

#include <algorithm>
#include <utility>

#include "MapMolecule.h"

#include "StyleMapMolecule.h"
using namespace vtk;

const StyleMapMolecule StyleMapMolecule::styleFast = {UnitRadius, 0.375f, true, false, SingleColor, 0.075f, {75, 75, 75}};

const StyleMapMolecule StyleMapMolecule::styleStyx = {UnitRadius, 0.250f, true, true, DiscreteByAtom, 0.250f, {75, 75, 75}};
const StyleMapMolecule StyleMapMolecule::styleCPK = {VDWRadius, 1.000f, false, false, SingleColor, 0.125f, {75, 75, 75}};
const StyleMapMolecule StyleMapMolecule::styleBnS = {UnitRadius, 0.375f, true, true, DiscreteByAtom, 0.125f, {75, 75, 75}};

bool StyleMapMolecule::operator==(const StyleMapMolecule &v) const
{
    return bool((styleAtomRadius_ == v.styleAtomRadius_)  //
                && (radAtomsScale_ == v.radAtomsScale_)   //
                && (styleBondColor_ == v.styleBondColor_) //
                && (bRenderMulti_ == v.bRenderMulti_)     //
                && (radBonds_ == v.radBonds_)             //
                && (colorBond_[0] == v.colorBond_[0])     //
                && (colorBond_[1] == v.colorBond_[1])     //
                && (colorBond_[2] == v.colorBond_[2])     //
    );
}

bool StyleMapMolecule::operator!=(const StyleMapMolecule &v) const
{
    return !this->operator==(v); // auto cast?
}

const char *StyleMapMolecule::AtomRadiusModeName(short mode)
{
    switch (mode)
    {
    case CovalentRadius:
        return "CovalentRadius";
    case VDWRadius:
        return "VDWRadius";
    case UnitRadius:
        return "UnitRadius";
    case CustomArrayRadius:
        return "CustomArrayRadius";
    default:
        return nullptr;
    }
}

void StyleMapMolecule::SetupMapMolecule(MapMolecule *pmap) const
{
    assert(pmap);
    // atoms
    pmap->SetAtomicRadiusType(styleAtomRadius_);
    pmap->SetAtomicRadiusScaleFactor(radAtomsScale_);
    // bonds
    pmap->SetRenderBonds(bRenderBonds_);
    pmap->SetBondColorMode(styleBondColor_);
    pmap->SetUseMultiCylindersForBonds(bRenderMulti_);
    pmap->SetBondRadius(radBonds_);
    pmap->SetBondColor(colorBond_[0], colorBond_[1], colorBond_[2]);
}