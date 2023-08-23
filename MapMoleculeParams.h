#ifndef Map_Molecule_Params_h__
#define Map_Molecule_Params_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER


struct MapMoleculeParams
{
    ///@{
    /**
   * Customize atom rendering
   */
    // bool RenderAtoms; // should atoms be rendered always?
    //
    bool RenderBonds;
    enum
    {
        CovalentRadius = 0,
        VDWRadius,
        UnitRadius,
        CustomArrayRadius
    };
    int AtomicRadiusType;
    enum
    {
        SingleColor = 0,
        DiscreteByAtom
    };
    int AtomColorMode;
    float AtomicRadiusScaleFactor;
    char *AtomicRadiusArrayName;
    unsigned char AtomColor[3];
    ///@}

    ///@{
    /**
   * Customize bond rendering
   */
    int BondColorMode;
    bool UseMultiCylindersForBonds;
    float BondRadius;
    unsigned char BondColor[3];
    ///@}

    void copyParameters(const MapMoleculeParams& p)
    {
        *this = p;
    }
};

#endif // !Map_Molecule_Params_h__
