#include "AcquireFileHIN.h"

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    AcquireFileHIN.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

using namespace vtk;

#include "Molecule.h"
#include "Elements.h"

#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPeriodicTable.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtksys/FStream.hxx>

#include <cmath>
#include <cstring>
#include <sstream>

//------------------------------------------------------------------------------
vtkStandardNewMacro(AcquireFileHIN);

//------------------------------------------------------------------------------
AcquireFileHIN::AcquireFileHIN() : AcquireFileBase(1) {}

//------------------------------------------------------------------------------
void AcquireFileHIN::PrintSelf(ostream &os, vtkIndent indent)
{
  os << indent << "HIN parser:\n";
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
// int AcquireFileHIN::RequestInformation(vtkInformation *vtkNotUsed(request),
//                                       vtkInformationVector **vtkNotUsed(inputVector),
//                                       vtkInformationVector *outputVector)
int AcquireFileHIN::ReadSizesFrom(InputFile &inp)
{
  String one_line;

  if (!GetLine(inp, one_line)) // first line: NumberOfAtoms
  {
    vtkErrorMacro("AcquireFileHIN error reading source string " << this->getPath().string());
    return 0;
  }

  vtkIdType nAtoms(0), nFragments(0);
  do
  {
    if (!one_line.find("atom"))
    {
      ++nAtoms;
      continue;
    }
    if (!one_line.find("mol"))
    {
      ++nFragments;
      continue;
    }
    /* code */
  } while (GetLine(inp, one_line));
  this->resetNumberOfAtoms(nAtoms);
  return (nAtoms > 0)
             ? 1
             : 0;
}

//------------------------------------------------------------------------------
// int AcquireFileHIN::RequestData(vtkInformation *vtkNotUsed(request),
//                                vtkInformationVector **vtkNotUsed(inputVector),
//                                vtkInformationVector *outputVector)
int AcquireFileHIN::ReadDataFrom(InputFile &inp, Molecule *ptrMol)
{
  // call base class:
  if (!this->Superclass::ReadDataFrom(inp, ptrMol))
    return 0;

  vtkIdType nAtoms;

  String one_line;

  if (!GetLine(inp, one_line)) // first line: NumberOfAtoms
  {
    vtkErrorMacro("AcquireFileHIN error reading source string " << this->getPath().string());
    return 0;
  }
  vtkIdType idFragFrom(0), idFragTill(0);
  do
  {
    InputString inps(one_line);
    String code, skip;
    inps >> code >> skip;
    if (!code.compare("mol")) // new molecule fragment
    {
      continue;
    }
    if (!code.compare("endmol")) // new molecule fragment
    {
      continue;
    }
    if (!code.compare("atom")) // atom string:
    {
      String atomType;
      double q, x, y, z;
      continue;
    }
    /* code */
  } while (GetLine(inp, one_line));

  return 1;
}

/*
//------------------------------------------------------------------------------
int AcquireFileHIN::OnReadDataComplete(Molecule *ptrMol)
{
    if (!this->Superclass::OnReadDataComplete(ptrMol) )
     return 0;

    vtkNew<Molecule> newmol;

    // Let's make some possible bonds:
    vtkNew<MakeBondsDistances> mk_bonds;
    mk_bonds->SetInputData(this->GetOutput());
    mk_bonds->SetOutput(newmol);
    mk_bonds->Update();

    ptrMol->DeepCopy(newmol);
    return (ptrMol->GetNumberOfAtoms() > 0 ? 1 : 0);
}
*/