#include "AcquireFileEXTOUT.h"

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    AcquireFileEXTOUT.cxx

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

#include <vtkType.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtksys/FStream.hxx>

#include <cmath>
#include <cstring>
#include <sstream>

//------------------------------------------------------------------------------
vtkStandardNewMacro(AcquireFileEXTOUT);

//------------------------------------------------------------------------------
AcquireFileEXTOUT::AcquireFileEXTOUT()
{
    this->SetNumberOfOutputPorts(2);
}

//------------------------------------------------------------------------------
int AcquireFileEXTOUT::RequestInformation(vtkInformation *vtkNotUsed(request),
                                          vtkInformationVector **vtkNotUsed(inputVector),
                                          vtkInformationVector *outputVector)
{
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    vtksys::ifstream fileInput(this->GetFileName());
    if (!fileInput.is_open())
    {
        vtkErrorMacro("AcquireFileEXTOUT error opening file: " << this->GetFileName());
        return 0;
    }

    if (ScrollStrings(fileInput, NumLinesHeader))
    {
        vtkErrorMacro("AcquireFileEXTOUT signaled an unexpected EOF in file: " << this->GetFileName());
        return 0;
    }
    vtkIdType idAtom = 0;

    std::string atom_line;
    do
    {
        if (!getline(fileInput, atom_line))
            break;
        ++idAtom;
    } while (!atom_line.empty());

    fileInput.close();

    // there is some additional information after

    return 1;
}

//------------------------------------------------------------------------------
int AcquireFileEXTOUT::RequestData(vtkInformation *vtkNotUsed(request),
                                   vtkInformationVector **vtkNotUsed(inputVector),
                                   vtkInformationVector *outputVector)
{
    vtkInformation *outInfo = outputVector->GetInformationObject(0);
    Molecule *output = Molecule::SafeDownCast(vtkDataObject::GetData(outputVector));

    if (!output)
    {
        vtkErrorMacro("AcquireFileEXTOUT does not have a Molecule as output.");
        return 1;
    }

    vtksys::ifstream fileInput(this->GetFileName());

    if (!fileInput.is_open())
    {
        vtkErrorMacro("AcquireFileEXTOUT error opening file: " << this->GetFileName());
        return 0;
    }
    if (ScrollStrings(fileInput, NumLinesHeader))
    {
        vtkErrorMacro("AcquireFileEXTOUT signaled an unexpected EOF in file: " << this->GetFileName());
        return 0;
    }
    vtkIdType idAtom = 0;

    // (re)construct a Molecule
    output->Initialize();

    std::string atom_line;
    if (!GetLine(fileInput, atom_line) || !*atom_line.begin())
        return 0;

    do
    {
        if (fileInput.fail()) // checking we are at end of line
        {
            vtkErrorMacro("AcquireFileEXTOUT error reading file: "
                          << this->GetFileName() << " Problem reading atomic positions.");
            break;
        }
        std::string atom_label;
        double x, y, z;
        std::istringstream inp(atom_line);
        inp >> atom_label >> x >> y >> z;
        output->AppendAtom(Elements::SymbolToNumber(atom_label.c_str()),
                           x, y, z);
        if (!GetLine(fileInput, atom_line))
        {
            vtkErrorMacro("AcquireFileEXTOUT reading premature file: "
                          << this->GetFileName());
            break;
        }

    } while (!atom_line.empty() && *atom_line.begin());
    fileInput.close();

    return 1;
}

//------------------------------------------------------------------------------
void AcquireFileEXTOUT::PrintSelf(ostream &os, vtkIndent indent)
{
    os << indent << "EXTOUT parser:\n";
    this->Superclass::PrintSelf(os, indent);
}
