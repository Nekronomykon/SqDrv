#include "Bond.h"
using namespace vtk;

/*=========================================================================

Program:   Visualization Toolkit
Module:    Bond.cxx
Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "Atom.h"
#include "Molecule.h"

#include "vtkMath.h"
#include "vtkVector.h"
#include "vtkVectorOperators.h"

#include <cassert>

//------------------------------------------------------------------------------
Bond::Bond(Molecule *parent, vtkIdType id, vtkIdType beginAtomId, vtkIdType endAtomId, vtkIdType idSpot)
    : molecule_(parent), Id(id), BeginAtomId(beginAtomId), EndAtomId(endAtomId), IdSpot_(idSpot)
{
    assert(parent != nullptr);
    assert(id < parent->GetNumberOfBonds());
    assert(beginAtomId < parent->GetNumberOfAtoms());
    assert(endAtomId < parent->GetNumberOfAtoms());
    assert(idSpot < 0 || idSpot >= parent->GetNumberOfAtoms());
}

//------------------------------------------------------------------------------
void Bond::PrintSelf(ostream &os, vtkIndent indent)
{
    os << indent << "molecule_: " << this->molecule_ << " Id: " << this->Id
       << " Order: " << this->GetOrder() << " Length: " << this->GetLength()
       << " BeginAtomId: " << this->BeginAtomId << " EndAtomId: " << this->EndAtomId << endl;
}

//------------------------------------------------------------------------------
double Bond::GetLength() const
{
    // Reimplement here to avoid the potential cost of building the EdgeList
    // (We already know the atomIds, no need to look them up)
    double pos1[3], pos2[3];
    this->molecule_->GetAtomPosition(this->BeginAtomId, pos1);
    this->molecule_->GetAtomPosition(this->EndAtomId, pos2);
    return std::sqrt(vtkMath::Distance2BetweenPoints(pos1, pos2));
}

//------------------------------------------------------------------------------
vtkIdType Bond::GetBeginAtomId() const { return this->BeginAtomId; }

//------------------------------------------------------------------------------
vtkIdType Bond::GetEndAtomId() const { return this->EndAtomId; }

//------------------------------------------------------------------------------
vtkIdType Bond::GetSpotId() const { return IdSpot_; }

//------------------------------------------------------------------------------
Atom Bond::GetBeginAtom() { return this->molecule_->GetAtom(this->BeginAtomId); }

//------------------------------------------------------------------------------
Atom Bond::GetEndAtom() { return this->molecule_->GetAtom(this->EndAtomId); }

//------------------------------------------------------------------------------
Atom Bond::GetBeginAtom() const { return this->molecule_->GetAtom(this->BeginAtomId); }

//------------------------------------------------------------------------------
Atom Bond::GetEndAtom() const { return this->molecule_->GetAtom(this->EndAtomId); }

//------------------------------------------------------------------------------
unsigned short Bond::GetOrder() { return this->molecule_->GetBondOrder(this->Id); }
