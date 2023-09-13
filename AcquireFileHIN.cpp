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
    vtkIdType idKey;
    inps >> code >> idKey;
    vtkIdType idBeginFrag(0);
    if (!code.find("mol")) // new molecule fragment
    {
      inps >> skip; // file name???
      continue;
    }
    if (!code.find("endmol")) // new molecule fragment
    {
      idBeginFrag = ptrMol->GetNumberOfAtoms();
      continue;
    }
    if (!code.find("atom")) // atom string:
    {
      // atom 1 - S   **  - 0  1.42151   2.56678   1.77484 4 2 d 4 s 10 s 11 s
      String atomType;
      char cSkip, cType;
      int nBonds, idTo;
      double q, x, y, z;
      // atom 1 - S   **  - 0  1.42151   2.56678   1.77484 4 2 d 4 s 10 s 11 s
      //       ^ now reading from here
      inps >> cSkip >> atomType >> skip >> cSkip >> q >> x >> y >> z >> nBonds;
      vtkAtom atom = ptrMol->AppendAtom(Elements::SymbolToNumber(atomType.c_str()), x, y, z);
      // atom 1 - S   **  - 0  1.42151   2.56678   1.77484 4 2 d 4 s 10 s 11 s
      //                                                    ^ now reading from here
      while (nBonds--)
      {
        inps >> idTo >> cType;
        idTo += idBeginFrag;
        if (--idTo >= atom.GetId())
          break;
        int kBondType = 0;
        if (cType == 's')
          kBondType = 1;
        else if (cType == 'd')
          kBondType = 2;
        else if (cType == 't')
          kBondType = 3;
        // whatever else...
        ptrMol->AppendBond(atom.GetId(), idTo, kBondType);
      }
      continue;
    }
  } while (GetLine(inp, one_line));

  return ptrMol->GetNumberOfAtoms() > 0
             ? 1
             : 0;
}
