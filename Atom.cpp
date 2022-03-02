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
Atom::Atom(Molecule* parent, vtkIdType id)
  : molecule_(parent)
  , Id(id)
{
  assert(parent != nullptr);
  assert(id < parent->GetNumberOfAtoms());
}

//------------------------------------------------------------------------------
void Atom::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "Molecule: " << this->molecule_ << " Id: " << this->Id
     << " Element: " << this->GetAtomicNumber() << " Position: " << this->GetPosition() << endl;
}

//------------------------------------------------------------------------------
unsigned short Atom::GetAtomicNumber() const
{
  return this->molecule_->GetAtomAtomicNumber(this->Id);
}

//------------------------------------------------------------------------------
void Atom::SetAtomicNumber(unsigned short atomicNum)
{
  this->molecule_->SetAtomAtomicNumber(this->Id, atomicNum);
}

//------------------------------------------------------------------------------
void Atom::GetPosition(float pos[3]) const
{
  this->molecule_->GetAtomPosition(this->Id, pos);
}

//------------------------------------------------------------------------------
void Atom::GetPosition(double pos[3]) const
{
  this->molecule_->GetAtomPosition(this->Id, pos);
}

//------------------------------------------------------------------------------
void Atom::SetPosition(const float pos[3])
{
  this->molecule_->SetAtomPosition(this->Id, vtkVector3f(pos));
}

//------------------------------------------------------------------------------
void Atom::SetPosition(float x, float y, float z)
{
  this->molecule_->SetAtomPosition(this->Id, x, y, z);
}

//------------------------------------------------------------------------------
vtkVector3f Atom::GetPosition() const
{
  return this->molecule_->GetAtomPosition(this->Id);
}

//------------------------------------------------------------------------------
void Atom::SetPosition(const vtkVector3f& pos)
{
  this->molecule_->SetAtomPosition(this->Id, pos);
}
