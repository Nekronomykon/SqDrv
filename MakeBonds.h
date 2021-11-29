#ifndef Make_Bonds_h__
#define Make_Bonds_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkMoleculeAlgorithm.h>
#include <vtkSimpleBondPerceiver.h>

namespace vtk
{
    typedef vtkSimpleBondPerceiver MakeBondsSimple;
}; // namespace vtk

#endif // !Make_Bonds_h__
