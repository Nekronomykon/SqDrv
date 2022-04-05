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
Bond::Bond(Molecule &parent, vtkIdType id, vtkIdType beginAtomId, vtkIdType endAtomId)
    : molecule_(parent), Id_(id), BeginAtomId(beginAtomId), EndAtomId(endAtomId), IdSpot_(id + parent.GetNumberOfAtoms())
{
    assert(id < parent.GetNumberOfBonds());
    assert(beginAtomId < parent.GetNumberOfAtoms());
    assert(endAtomId < parent.GetNumberOfAtoms());
    assert(beginAtomId != endAtomId);
}

//------------------------------------------------------------------------------
void Bond::PrintSelf(ostream &os, vtkIndent indent)
{
    os << indent << "Molecule Id: " << this->Id_
       << " Order: " << this->GetOrder() 
       << " Length: " << this->GetLength()
       << " BeginAtomId: " << this->BeginAtomId 
       << " EndAtomId: " << this->EndAtomId << endl;
}

//------------------------------------------------------------------------------
double Bond::GetLength() const
{
    // Reimplement here to avoid the potential cost of building the EdgeList
    // (We already know the atomIds, no need to look them up)
    double pos1[3], pos2[3];
    molecule_.GetAtomPosition(this->BeginAtomId, pos1);
    molecule_.GetAtomPosition(this->EndAtomId, pos2);
    return std::sqrt(vtkMath::Distance2BetweenPoints(pos1, pos2));
}

//------------------------------------------------------------------------------
vtkIdType Bond::GetBeginAtomId() const { return this->BeginAtomId; }

//------------------------------------------------------------------------------
vtkIdType Bond::GetEndAtomId() const { return this->EndAtomId; }

//------------------------------------------------------------------------------
vtkIdType Bond::GetSpotId() const { return this->IdSpot_; }

//------------------------------------------------------------------------------
Atom Bond::GetBeginAtom() { return molecule_.GetAtom(this->BeginAtomId); }

//------------------------------------------------------------------------------
Atom Bond::GetEndAtom() { return molecule_.GetAtom(this->EndAtomId); }

//------------------------------------------------------------------------------
Atom Bond::GetBeginAtom() const { return molecule_.GetAtom(this->BeginAtomId); }

//------------------------------------------------------------------------------
Atom Bond::GetEndAtom() const { return molecule_.GetAtom(this->EndAtomId); }

//------------------------------------------------------------------------------
BondOrder Bond::GetOrder() { return molecule_.GetBondOrder(this->Id_); }
