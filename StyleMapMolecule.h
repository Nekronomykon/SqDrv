#ifndef Style_MapMolecule_h
#define Style_MapMolecule_h

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

// #include <vtkColor.h>

class vtkMoleculeMapper;

namespace vtk
{

  // class StyleMapMolecule;

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
    bool bRenderMulti_;
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
      std::swap(rad_type, styleAtomRadius_);
      return rad_type;
    }
    static const char *AtomRadiusModeName(short mode);
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
      std::swap(sc, radAtomsScale_);
      return sc;
    }
    //
    bool HasToRenderBonds() const { return bRenderBonds_; }
    void RenderBondsOn() { bRenderBonds_ = true; }
    void RenderBondsOff() { bRenderBonds_ = false; }
    void ToggleRenderBonds() { bRenderBonds_ = !bRenderBonds_; }
    //
    bool IsMultiBonds() const { return bRenderMulti_; }
    void MultiBondsOn() { bRenderMulti_ = true; }
    void MultiBondsOff() { bRenderMulti_ = false; }
    bool ToggleMultiBonds()
    {
      bRenderMulti_ = !bRenderMulti_;
      return bRenderMulti_;
    }
    //
    short GetTypeBondsColor() const { return styleBondColor_; }
    short ResetTypeBondsColor(short col_type)
    {
      std::swap(col_type, styleBondColor_);
      return col_type;
    }

    static const char *BondColorModeName(short mode)
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
      std::swap(rb, radBonds_);
      return rb;
    }
    //
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // Just in case the application of the external style upon the VTK class
    // vtkMoleculeMapper
    //
    void SetupMoleculeMapper(vtkMoleculeMapper *pmap) const;
    //
    //////////////////////////////////////////////////////////////////////////
  };

} // namespace vtk

#endif // Style_MapMolecule_h
