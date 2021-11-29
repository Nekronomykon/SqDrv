#ifndef Molecule_Mapper_h__
#define Molecule_Mapper_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "StyleMapMolecule.h"
#include "Molecule.h"
#include <vtkMoleculeMapper.h>

namespace vtk
{

    typedef vtkMoleculeMapper MoleculeMapper;

}; // namespace vtk

#endif // !Molecule_Mapper_h__
