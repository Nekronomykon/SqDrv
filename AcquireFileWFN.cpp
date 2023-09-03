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
// int AcquireFileWFN::RequestInformation(vtkInformation *vtkNotUsed(request),
//                                       vtkInformationVector **vtkNotUsed(inputVector),
//                                       vtkInformationVector *outputVector)
int AcquireFileWFN::ReadSizesFrom(InputFile &inp)
{
  /*
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  */

  GetLine(inp, this->Title()); // first (title) line may be empty

  std::string one_line;
  int nOrb, nPrim;

  // second line: NumberOfAtoms is the rightmost of the numeric fields
  if (!GetLine(inp, one_line) || one_line.empty()) // second line: Sizes
  {
    vtkErrorMacro("AcquireFileXYZ error reading (atomic) size from: " << this->getPath().string());
    return 0;
  }
  else
  {
    InputString inp_na(one_line);
    String skip;
    inp_na >> skip                // GTO
        >> nOrb >> skip           // MOL
        >> skip                   // ORBITALS
        >> nPrim >> skip          // PRIMITIVES
        >> this->NumberOfAtoms(); // ignoring rest of the line (NUCLEI)
  }
  int na = this->GetNumberOfAtoms();

  for (; na; --na)
  {
    if (!GetLine(inp, one_line) || one_line.empty())
      break; // for each atom a line with symbol, x, y, z
  }

  return (!na) ? 1 : 0;
}

//------------------------------------------------------------------------------
int AcquireFileWFN::RequestData(vtkInformation *vtkNotUsed(request),
                                vtkInformationVector **vtkNotUsed(inputVector),
                                vtkInformationVector *outputVector)
{
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  Molecule *output = Molecule::SafeDownCast(vtkDataObject::GetData(outputVector));

  if (!output)
  {
    vtkErrorMacro("AcquireFileWFN does not have a Molecule as output.");
    return 1;
  }

  InputFile fileInput(this->getPath());

  if (!fileInput.is_open())
  {
    vtkErrorMacro("AcquireFileWFN error opening file: " << this->getPath().string());
    return 0;
  }

  std::string one_line; // read this string from disk
  // and parse it using istringstream and state

  GetLine(fileInput, one_line); // first line: Title
  if (!fileInput)
  {
    vtkErrorMacro("AcquireFileWFN signaled an unexpected EOF in file: " << this->getPath().string());
    return 0;
  }

  if (!GetLine(fileInput, one_line) || one_line.empty()) // second line: Sizes
  {
    vtkErrorMacro("AcquireFileXYZ error reading (atomic) size from: " << this->getPath().string());
    return 0;
  }

  // reconstruct Molecule
  output->Initialize();

  // by strings, and strings by atoms:
  vtkIdType nAtoms = this->GetNumberOfAtoms();
  for (int i = 0; i < nAtoms; i++)
  {
    if (!GetLine(fileInput, one_line) || one_line.empty()) // first line: NumberOfAtoms
    {
      vtkErrorMacro("AcquireFileXYZ error reading (atomic) size from: " << this->getPath().string());
      return 0;
    }
    if (fileInput.fail()) // checking we are at end of line
    {
      vtkErrorMacro("AcquireFileWFN error reading file: "
                    << this->getPath().string() << " Problem reading atoms' positions.");
      fileInput.close();
      return 0;
    }

    std::istringstream inp_na(one_line);
    std::string atomType;
    std::string skip;
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
    output->AppendAtom((unsigned short)q, x, y, z);
  }
  fileInput.close();

  return 1;
}

//------------------------------------------------------------------------------
void AcquireFileWFN::PrintSelf(ostream &os, vtkIndent indent)
{
  os << indent << "WFN parser:" << endl;
  this->Superclass::PrintSelf(os, indent);
}
