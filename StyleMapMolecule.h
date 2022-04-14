#ifndef Style_MapMolecule_h
#define Style_MapMolecule_h

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "Elements.h"

namespace vtk
{

  class MapMolecule;

  // static bool operator==(const StyleMapMolecule &, const StyleMapMolecule &);
  // static bool operator!=(const StyleMapMolecule &, const StyleMapMolecule &);

  struct StyleMapMolecule
  {
    // private:
    // atoms
    // bool bRenderAtoms_; // --> always render if available
    short styleAtomRadius_;
    float radAtomsScale_;
    // bonds
    bool bRenderBonds_;
    bool bBondsMulticylinder_;
    short styleBondColor_;
    float radBonds_;
    unsigned char colorBond_[3]; // transform to color4?

  public:
    bool operator==(const StyleMapMolecule &) const;
    bool operator!=(const StyleMapMolecule &) const;

    enum
    {
      CovalentRadius = 0,
      VDWRadius,
      UnitRadius,
      CustomArrayRadius
    };

    enum
    {
      SingleColor = 0,
      DiscreteByAtom
    };

    static const StyleMapMolecule styleCPK;
    static const StyleMapMolecule styleBnS;
    static const StyleMapMolecule styleFast;
    static const StyleMapMolecule styleStyx;

    //////////////////////////////////////////////////////////////////////////
    //
    // bool HasToRenderAtoms() const { return bRenderAtoms_; }
    // void RenderAtomsOn() { bRenderAtoms_ = true; }
    // void RenderAtomsOff() { bRenderAtoms_ = false; }
    // bool ToggleRenderAtoms() { bRenderAtoms_ = !bRenderAtoms_; return bRenderAtoms_; }
    //
    short GetAtomicRadiusType() const { return styleAtomRadius_; }
    short ResetAtomicRadiusType(short rad_type)
    {
      short int res = styleAtomRadius_;
      styleAtomRadius_ = rad_type;
      return res;
    }
    static inline const char *AtomRadiusModeName(short mode)
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

    const char *GetAtomicRadiusTypeAsString() const
    {
      const char *res = AtomRadiusModeName(this->GetAtomicRadiusType());
      return (!res) ? "InvalidType" : res;
    }
    //
    float GetAtomicRadiusScale() const
    {
      return radAtomsScale_;
    }
    float ResetAtomicRadiusScale(float sc)
    {
      float res = radAtomsScale_;
      radAtomsScale_ = sc;
      return res;
    }
    //
    bool HasToRenderBonds() const { return bRenderBonds_; }
    void SetRenderBonds(bool bYes = true) { bRenderBonds_ = bYes; }
    void RenderBondsOn() { bRenderBonds_ = true; }
    void RenderBondsOff() { bRenderBonds_ = false; }
    void ToggleRenderBonds() { bRenderBonds_ = !bRenderBonds_; }
    //
    bool AreBondsMulticylinder() const { return bBondsMulticylinder_; }
    void SetBondsMulticylinder(bool bYes = true) { bBondsMulticylinder_ = bYes; }
    void BondsMulticylinderOn() { bBondsMulticylinder_ = true; }
    void BondsMulticylinderOff() { bBondsMulticylinder_ = false; }
    bool ToggleBondsMulticylinder()
    {
      bBondsMulticylinder_ = !bBondsMulticylinder_;
      return bBondsMulticylinder_;
    }

    //
    short GetTypeBondsColor() const { return styleBondColor_; }
    short ResetTypeBondsColor(short int col_type)
    {
      short int res = styleBondColor_;
      styleBondColor_ = col_type;
      return res;
    }

    static inline const char *BondColorModeName(short mode)
    {
      switch (mode)
      {
      case SingleColor:
        return "SingleColor";
      case DiscreteByAtom:
        return "DiscreteByAtom";
      default:
        return nullptr;
      }
    }

    const char *GetBondColorModeAsString() const
    {
      const char *res = BondColorModeName(this->GetTypeBondsColor());
      return (!res) ? "InvalidType" : res;
    }
    //
    float GetBondRadius() const { return radBonds_; }
    float ReserBondRadius(float rb)
    {
      float res = radBonds_;
      radBonds_ = rb;
      return res;
    }
    //
    //////////////////////////////////////////////////////////////////////////

    void SetupMapMolecule(MapMolecule * /*pMap*/) const;
  }; // struct StyleMapMolecule
  //
  //////////////////////////////////////////////////////////////////////////
}; // namespace vtk

#endif // Style_MapMolecule_h
