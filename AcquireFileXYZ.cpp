#include "AcquireFileXYZ.h"

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    AcquireFileXYZ.cxx

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
vtkStandardNewMacro(AcquireFileXYZ);

//------------------------------------------------------------------------------
AcquireFileXYZ::AcquireFileXYZ() : AcquireMoleculeFile(0, 1) {}

//------------------------------------------------------------------------------
// int AcquireFileXYZ::RequestInformation(vtkInformation *vtkNotUsed(request),
//                                       vtkInformationVector **vtkNotUsed(inputVector),
//                                       vtkInformationVector *outputVector)
int AcquireFileXYZ::ReadSizesFrom(InputFile &inp)
{
    /*
        vtkInformation *outInfo = outputVector->GetInformationObject(0);

        vtksys::ifstream inp(this->GetFileName());

        if (!inp.is_open())
        {
            vtkErrorMacro("AcquireFileXYZ error opening file: " << this->GetFileName());
            return 0;
        }

        if (!this->Superclass::ReadSizesFrom(inp))
            eturn 0;
    */

    std::string one_line;

    if (!GetLine(inp, one_line) || one_line.empty()) // first line: NumberOfAtoms
    {
        vtkErrorMacro("AcquireFileXYZ error reading (atomic) size from: " << this->GetFileName());
        return 0;
    }
    else
    {
        InputString inp_na(one_line);
        inp_na >> this->NumberOfAtoms(); // ignoring rest of the line
    }
    int na = this->GetNumberOfAtoms();

    if (!na)
    {
        vtkErrorMacro("AcquireFileXYZ error setting (atomic) size from: " << this->GetFileName());
        return 0;
    }

    /////////////////////////////////////////////////////////////////////////////////
    // ~?~ size_t nReadAtoms = this->ReadStructure(inp, nullptr, context);
    //                                             ^ from ^    ^ to ^
    // ~?~ or somewhat similar...
    //

    GetLine(inp, this->NameOfStructure()); // second (title) line may be empty
    
    // simply scroll NumberOfAtom lines of the file:
    for (; na; --na)
    {
        if (!GetLine(inp, one_line) || one_line.empty())
            break; // for each atom a line with symbol, x, y, z
    }
    //
    /////////////////////////////////////////////////////////////////////////////////

    return (!na) ? 1 : 0;
}

//------------------------------------------------------------------------------
int AcquireFileXYZ::RequestData(vtkInformation *vtkNotUsed(request),
                                vtkInformationVector **vtkNotUsed(inputVector),
                                vtkInformationVector *outputVector)
{
    vtkInformation *outInfo = outputVector->GetInformationObject(0);
    Molecule *output = Molecule::SafeDownCast(vtkDataObject::GetData(outputVector));

    if (!output)
    {
        vtkErrorMacro("AcquireFileXYZ does not have a Molecule as output.");
        return 1;
    }

    vtksys::ifstream inp(this->GetFileName());

    if (!inp.is_open())
    {
        vtkErrorMacro("AcquireFileXYZ error opening file: " << this->GetFileName());
        return 0;
    }

    vtkIdType nAtoms;

    std::string one_line;

    if (!GetLine(inp, one_line) || one_line.empty()) // first line: NumberOfAtoms
    {
        vtkErrorMacro("AcquireFileXYZ error reading (atomic) size from: " << this->GetFileName());
        return 0;
    }
    else
    {
        std::istringstream inp_na(one_line);
        inp_na >> nAtoms; // ignoring rest of the line
    }

    if (nAtoms != this->GetNumberOfAtoms())
    {
        vtkErrorMacro("AcquireFileXYZ error: inconsistent atomic sizes while rereading " << this->GetFileName());
        return 0;
    }

    if (!GetLine(inp, one_line))
    {
        vtkErrorMacro("AcquireFileXYZ error: unexpected EOF while taking to atoms of " << this->GetFileName());
        return 0;
    }

    // reconstruct Molecule
    output->Initialize();
    // output-SetNumberOfAtoms(this->GetNumberOfAtoms());

    for (int i = 0; i < nAtoms; i++)
    {
        if (!GetLine(inp, one_line))
        {
            vtkErrorMacro("AcquireFileXYZ error: unexpected EOF while reading atom "
                          << i + 1
                          << " of overall " << nAtoms
                          << " from " << this->GetFileName());
            return 0;
        }
        std::istringstream inp_atom(one_line);
        std::string atomType;
        float x, y, z;
        inp_atom >> atomType >> x >> y >> z;
        if (inp.fail()) // checking we are at end of line
        {
            vtkErrorMacro("AcquireFileXYZ error reading file: "
                          << this->GetFileName() << " Problem reading atoms' positions.");
            inp.close();
            return 0;
        }
        output->AppendAtom(Elements::SymbolToNumber(atomType.c_str()),
                           x, y, z);
    }
    inp.close();

    return 1;
}

//------------------------------------------------------------------------------
void AcquireFileXYZ::PrintSelf(ostream &os, vtkIndent indent)
{
    os << indent << "XYZ parser:\n";
    this->Superclass::PrintSelf(os, indent);
}
