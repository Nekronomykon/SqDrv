#ifndef Molecule_h__
#define Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "Elements.h"

#include <vtkMolecule.h>
using Molecule = vtkMolecule;
typedef vtkNew<Molecule> NewMolecule;
typedef vtkSmartPointer<Molecule> AMolecule;

#endif // !Molecule_h__
