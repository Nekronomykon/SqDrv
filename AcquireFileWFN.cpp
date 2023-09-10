#include "AcquireFileWFN.h"

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    AcquireFileWFN.cxx

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

#include <cmath>
#include <cstring>
#include <sstream>

//------------------------------------------------------------------------------
vtkStandardNewMacro(AcquireFileWFN);

//------------------------------------------------------------------------------
AcquireFileWFN::AcquireFileWFN() : AcquireFileBase(1) {}

//------------------------------------------------------------------------------
int AcquireFileWFN::ReadSizesFrom(InputFile &inp)
{
  /*
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  */

  GetLine(inp, this->Title()); // first (title) line may be empty

  String one_line;
  int nOrb, nPrim;

  // second line: NumberOfAtoms is the rightmost of the numeric fields
  if (!GetLine(inp, one_line) || one_line.empty()) // second line: Sizes
  {
    vtkErrorMacro("AcquireFileXYZ error reading (atomic) size from: " << this->getPath().string());
    return 0;
  }

  InputString inp_na(one_line);
  String skip;
  inp_na >> skip                // GTO
      >> nOrb >> skip           // MOL
      >> skip                   // ORBITALS
      >> nPrim >> skip          // PRIMITIVES
      >> this->NumberOfAtoms(); // ignoring rest of the line (NUCLEI)

  int na = this->GetNumberOfAtoms();

  for (; na; --na)
  {
    if (!GetLine(inp, one_line) || one_line.empty())
      break; // for each atom a line with symbol, x, y, z
  }

  return (!na) ? 1 : 0;
}

//------------------------------------------------------------------------------
int AcquireFileWFN::ReadDataFrom(InputFile &inp, Molecule *ptrMol)
{
  // call base class:
  if (!this->Superclass::ReadDataFrom(inp, ptrMol))
    return 0;

  String one_line; // read this string from disk
  // and parse it using istringstream and state

  GetLine(inp, one_line); // first line: Title
  if (!inp)
  {
    vtkErrorMacro("AcquireFileWFN signaled an unexpected EOF in file: " << this->getPath().string());
    return 0;
  }

  if (!GetLine(inp, one_line) || one_line.empty()) // second line: Sizes
  {
    vtkErrorMacro("AcquireFileXYZ error reading (atomic) size from: " << this->getPath().string());
    return 0;
  }

  // reconstruct Molecule
  ptrMol->Initialize();

  // by strings, and strings by atoms:
  vtkIdType nAtoms = this->GetNumberOfAtoms();
  for (int i = 0; i < nAtoms; i++)
  {
    if (!GetLine(inp, one_line) || one_line.empty()) // first line: NumberOfAtoms
    {
      vtkErrorMacro("AcquireFileXYZ error reading (atomic) size from: " << this->getPath().string());
      return 0;
    }
    if (inp.fail()) // checking we are at end of line
    {
      vtkErrorMacro("AcquireFileWFN error reading file: "
                    << this->getPath().string() << " Problem reading atoms' positions.");
      inp.close();
      return 0;
    }

    InputString inp_na(one_line);
    String atomType;
    String skip;
    int nSkip;
    char cSkip;
    float x, y, z, q;
    inp_na >> atomType >> nSkip // # == i + 1
        >> skip                 // (CENTRE
        >> nSkip                // # == i + 1
        >> skip                 // )
        >> x >> y >> z >> skip  // CHARGE
        >> skip                 // '='
        >> q;                   // << now the EOL is here
    ptrMol->AppendAtom((unsigned short)q, x, y, z);
  }
  inp.close();

  return 1;
}

//------------------------------------------------------------------------------
void AcquireFileWFN::PrintSelf(ostream &os, vtkIndent indent)
{
  os << indent << "WFN parser:" << endl;
  this->Superclass::PrintSelf(os, indent);
}
