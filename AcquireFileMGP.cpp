#include "AcquireFileMGP.h"
// using namespace vtk;

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    AcquireFileMGP.cxx

  Copyright (c) ScrewDriver te Blackheadborough
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

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
vtkStandardNewMacro(AcquireFileMGP);

//------------------------------------------------------------------------------
AcquireFileMGP::AcquireFileMGP() {}


//------------------------------------------------------------------------------
void AcquireFileMGP::PrintSelf(ostream &os, vtkIndent indent)
{
    os << indent << "An MGP parser:\n";
    this->Superclass::PrintSelf(os, indent);
}
