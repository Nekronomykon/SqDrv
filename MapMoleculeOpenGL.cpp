#include "MapMoleculeOpenGL.h"
using namespace vtk;

/*=========================================================================

  Program:   Visualization Toolkit

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <vtkOpenGLSphereMapper.h>
#include <vtkOpenGLStickMapper.h>

#include "Molecule.h"
#include "Elements.h"

#include <vtkEventForwarderCommand.h>
#include <vtkGlyph3DMapper.h>
#include <vtkLookupTable.h>
#include "vtkObjectFactory.h"
#include "vtkTrivialProducer.h"

#include <vtkPeriodicTable.h>

//------------------------------------------------------------------------------
vtkStandardNewMacro(MapMoleculeOpenGL);

//------------------------------------------------------------------------------
void MapMoleculeOpenGL::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "FastAtomMapper:\n";
  this->FastAtomMapper->PrintSelf(os, indent.GetNextIndent());
  os << indent << "FastBondMapper:\n";
  this->FastBondMapper->PrintSelf(os, indent.GetNextIndent());
}

//------------------------------------------------------------------------------
MapMoleculeOpenGL::MapMoleculeOpenGL()
{
  // Setup glyph mappers
  // this->FastAtomMapper->SetScalarRange(0, Elements::NumberOfElements());
  StyleMapMolecule::SetupMapElements(this->FastAtomMapper);
  this->FastAtomMapper->SetColorModeToMapScalars();
  this->FastAtomMapper->SetScalarModeToUsePointFieldData();

  // this->FastBondMapper->SetScalarRange(0, Elements::NumberOfElements());
  StyleMapMolecule::SetupMapElements(this->FastBondMapper);

  // Forward commands to instance mappers
  vtkNew<vtkEventForwarderCommand> cb;
  cb->SetTarget(this);

  this->FastAtomMapper->AddObserver(vtkCommand::StartEvent, cb);
  this->FastAtomMapper->AddObserver(vtkCommand::EndEvent, cb);
  this->FastAtomMapper->AddObserver(vtkCommand::ProgressEvent, cb);

  this->FastBondMapper->AddObserver(vtkCommand::StartEvent, cb);
  this->FastBondMapper->AddObserver(vtkCommand::EndEvent, cb);
  this->FastBondMapper->AddObserver(vtkCommand::ProgressEvent, cb);

  // Connect the trivial producers to forward the glyph polydata
  this->FastAtomMapper->SetInputConnection(this->AtomGlyphPointOutput->GetOutputPort());
  this->FastBondMapper->SetInputConnection(this->BondGlyphPointOutput->GetOutputPort());
}

MapMoleculeOpenGL::~MapMoleculeOpenGL() = default;

//------------------------------------------------------------------------------
void MapMoleculeOpenGL::Render(vtkRenderer *ren, vtkActor *act)
{
  // Update cached polydata if needed
  this->UpdateGlyphPolyData();

  // Pass rendering call on
  this->FastAtomMapper->Render(ren, act);

  if ( this->GetStyle()->HasToRenderBonds() )
  {
    this->FastBondMapper->Render(ren, act);
  }

  /* if (this->RenderLattice)
  {
    this->LatticeMapper->Render(ren, act);
  }*/
}

void MapMoleculeOpenGL::ProcessSelectorPixelBuffers(
    vtkHardwareSelector *sel, std::vector<unsigned int> &pixeloffsets, vtkProp *prop)
{
  // forward to helper
  this->FastAtomMapper->ProcessSelectorPixelBuffers(sel, pixeloffsets, prop);

  if ( this->GetStyle()->HasToRenderBonds() )
  {
    this->FastBondMapper->ProcessSelectorPixelBuffers(sel, pixeloffsets, prop);
  }

  /* if (this->RenderLattice)
  {
    this->LatticeMapper->ProcessSelectorPixelBuffers(sel, pixeloffsets, prop);
  } */
}

//------------------------------------------------------------------------------
void MapMoleculeOpenGL::ReleaseGraphicsResources(vtkWindow *w)
{
  this->FastAtomMapper->ReleaseGraphicsResources(w);
  this->FastBondMapper->ReleaseGraphicsResources(w);
  this->Superclass::ReleaseGraphicsResources(w);
}

//------------------------------------------------------------------------------
// Generate scale and position information for each atom sphere
void MapMoleculeOpenGL::UpdateAtomGlyphPolyData()
{
  this->Superclass::UpdateAtomGlyphPolyData();
  this->FastAtomMapper->SetScalarMode(this->AtomGlyphMapper->GetScalarMode());
  this->FastAtomMapper->SetLookupTable(this->AtomGlyphMapper->GetLookupTable());
  this->FastAtomMapper->SetScaleArray("Scale Factors");

  // Copy the color array info:
  this->FastAtomMapper->SelectColorArray(this->AtomGlyphMapper->GetArrayId());
}

//------------------------------------------------------------------------------
// Generate position, scale, and orientation vectors for each bond cylinder
void MapMoleculeOpenGL::UpdateBondGlyphPolyData()
{
  this->Superclass::UpdateBondGlyphPolyData();

  this->FastBondMapper->SetLookupTable(this->BondGlyphMapper->GetLookupTable());
  this->FastBondMapper->SetScalarMode(this->BondGlyphMapper->GetScalarMode());
  this->FastBondMapper->SetColorMode(this->BondGlyphMapper->GetColorMode());
  this->FastBondMapper->SelectColorArray(this->BondGlyphMapper->GetArrayId());
  // Setup glypher
  this->FastBondMapper->SetScaleArray("Scale Factors");
  this->FastBondMapper->SetOrientationArray("Orientation Vectors");
  this->FastBondMapper->SetSelectionIdArray("Selection Ids");
}

//------------------------------------------------------------------------------
void MapMoleculeOpenGL::SetMapScalars(bool map)
{
  this->Superclass::SetMapScalars(map);
  this->FastAtomMapper->SetColorMode(
      map ? VTK_COLOR_MODE_MAP_SCALARS : VTK_COLOR_MODE_DIRECT_SCALARS);
  this->FastBondMapper->SetColorMode(
      map ? VTK_COLOR_MODE_MAP_SCALARS : VTK_COLOR_MODE_DIRECT_SCALARS);
}
