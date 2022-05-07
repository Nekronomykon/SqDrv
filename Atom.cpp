#include "Atom.h"
using namespace vtk;

/*=========================================================================

Program:   Visualization Toolkit
Module:    Atom.cxx
Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "Molecule.h"

#include <vtkVector.h>
#include <vtkVectorOperators.h>

#include <cassert>

//------------------------------------------------------------------------------
Atom::Atom(Molecule& parent, vtkIdType id)
  : molecule_(parent)
  , Id_(id)
{
  assert(id < parent.GetNumberOfAtoms());
}

//------------------------------------------------------------------------------
void Atom::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "Molecule Id: " << this->Id_
     << " Element: " << this->GetAtomTypeId() 
     << " Position: " << this->GetPosition() << endl;
}

//------------------------------------------------------------------------------
IdAtomType Atom::GetAtomTypeId() const
{
  return molecule_.GetAtomTypeId(this->Id_);
}

//------------------------------------------------------------------------------
void Atom::SetAtomTypeId(IdAtomType idType)
{
  molecule_.SetAtomTypeId(this->Id_, idType);
}

//------------------------------------------------------------------------------
void Atom::GetPosition(float pos[3]) const
{
  molecule_.GetAtomPosition(this->Id_, pos);
}

//------------------------------------------------------------------------------
void Atom::GetPosition(double pos[3]) const
{
  molecule_.GetAtomPosition(this->Id_, pos);
}

//------------------------------------------------------------------------------
void Atom::SetPosition(const float pos[3])
{
  molecule_.SetAtomPosition(this->Id_, vtkVector3f(pos));
}

//------------------------------------------------------------------------------
void Atom::SetPosition(float x, float y, float z)
{
  molecule_.SetAtomPosition(this->Id_, x, y, z);
}

//------------------------------------------------------------------------------
vtkVector3f Atom::GetPosition() const
{
  return molecule_.GetAtomPosition(this->Id_);
}

//------------------------------------------------------------------------------
void Atom::SetPosition(const vtkVector3f& pos)
{
  molecule_.SetAtomPosition(this->Id_, pos);
}

//------------------------------------------------------------------------------
//void Atom::SetPosition(const vtkVector3d &pos)
//{
//  this->molecule_->SetAtomPosition(this->Id, pos);
//}

