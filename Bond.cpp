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
Bond::Bond(Molecule &parent, vtkIdType idBond, vtkIdType idBegin, vtkIdType idEnd)
    : molecule_(parent), IdBond_(idBond), IdAtomBegin_(idBegin), IdAtomEnd_(idEnd), IdSpot_(-1)
{
    assert(idBond < parent.GetNumberOfBonds());
    assert(idBegin < parent.GetNumberOfAtoms());
    assert(idEnd < parent.GetNumberOfAtoms());
    assert(idBegin != idEnd);
}

//------------------------------------------------------------------------------
void Bond::PrintSelf(ostream &os, vtkIndent indent)
{
    os << indent << "Molecule Id: " << this->IdBond_
       << " Order: " << this->GetOrder() 
       << " Length: " << this->GetLength()
       << " IdAtom: Begin = " << this->IdAtomBegin_ 
       << " End = " << this->IdAtomEnd_ 
       << " Id Total: " << this->IdSpot_
       << endl;
}

//------------------------------------------------------------------------------
double Bond::GetLength() const
{
    // Reimplement here to avoid the potential cost of building the EdgeList
    // (We already know the atomIds, no need to look them up)
    double pos1[3], pos2[3];
    molecule_.GetAtomPosition(this->IdAtomBegin_, pos1);
    molecule_.GetAtomPosition(this->IdAtomEnd_, pos2);
    return std::sqrt(vtkMath::Distance2BetweenPoints(pos1, pos2));
}

//------------------------------------------------------------------------------
vtkIdType Bond::GetBeginAtomId() const { return this->IdAtomBegin_; }

//------------------------------------------------------------------------------
vtkIdType Bond::GetEndAtomId() const { return this->IdAtomEnd_; }

//------------------------------------------------------------------------------
vtkIdType Bond::GetSpotId() const { return this->IdSpot_; }

//------------------------------------------------------------------------------
Atom Bond::GetBeginAtom() { return molecule_.GetAtom(this->IdAtomBegin_); }

//------------------------------------------------------------------------------
Atom Bond::GetEndAtom() { return molecule_.GetAtom(this->IdAtomEnd_); }

//------------------------------------------------------------------------------
Atom Bond::GetBeginAtom() const { return molecule_.GetAtom(this->IdAtomBegin_); }

//------------------------------------------------------------------------------
Atom Bond::GetEndAtom() const { return molecule_.GetAtom(this->IdAtomEnd_); }

//------------------------------------------------------------------------------
BondOrder Bond::GetOrder() { return molecule_.GetBondOrder(this->IdBond_); }
