#include "AcquireFileWFX.h"
// using namespace vtk;

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    AcquireFileWFX.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "Molecule.h"
#include "Elements.h"

#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPeriodicTable.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtksys/FStream.hxx>

#include <vtkAtom.h>

#include <cmath>
#include <cstring>
#include <sstream>

//------------------------------------------------------------------------------
vtkStandardNewMacro(AcquireFileWFX);

//------------------------------------------------------------------------------
AcquireFileWFX::AcquireFileWFX() : AcquireFileBase(1) {}

int AcquireFileWFX::ReadSizesFrom(InputFile &inp)
{
  int nAtoms, nPrim, nOrb;

  String str_NumAtoms = GetMarkupContent(inp, "Number of Nuclei");

  // second line: NumberOfAtoms is the rightmost of the numeric fields
  if (str_NumAtoms.empty()) // Number of nuclei
  {
    vtkErrorMacro("AcquireFileWFX error reading (atomic) size from: " << this->getPath().string());
    return 0;
  }

  InputString inp_na(str_NumAtoms);
  inp_na >> this->NumberOfAtoms();

  return (this->GetNumberOfAtoms()) ? 1 : 0;
}

//------------------------------------------------------------------------------
int AcquireFileWFX::ReadDataFrom(InputFile &inp, Molecule *ptrMol)
{
  // call base class:
  if (!this->Superclass::ReadDataFrom(inp, ptrMol))
    return 0;

  vtkIdType nAtoms = this->GetNumberOfAtoms();
  String strNumbers = GetMarkupContent(inp, "Atomic Numbers");
  if (strNumbers.empty())
    return 0;
  InputString inpsNumbers(strNumbers);
  String strXYZ = GetMarkupContent(inp, "Nuclear Cartesian Coordinates");
  if (strXYZ.empty())
    return 0;
  InputString inpsXYZ(strXYZ);

  for (int i = 0; i < nAtoms; ++i)
  {
    IndexElement idxNew(0);
    inpsNumbers >> idxNew;
    if (idxNew > 0)
    {
      vtkAtom aNewAtom = ptrMol->AppendAtom();
      aNewAtom.SetAtomicNumber(idxNew);
      double x, y, z;
      inpsXYZ >> x >> y >> z;
      aNewAtom.SetPosition(x, y, z);
    }
  }
  return 1;
}

//------------------------------------------------------------------------------
void AcquireFileWFX::PrintSelf(ostream &os, vtkIndent indent)
{
  os << indent << "WFX parser:" << endl;
  this->Superclass::PrintSelf(os, indent);
}
