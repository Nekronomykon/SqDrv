#ifndef Bond_h
#define Bond_h
/*=========================================================================

  Program:   Visualization Toolkit
  Module:    Bond.h

  Copyright (c) ScrewDriver te Blackheadborough
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

/**
 * @class   Bond class
 * @brief   convenience proxy for Molecule
 *
 */

#include <vtkObject.h> // For macros, etc

#include "Atom.h" // For Atom

namespace vtk
{
  typedef unsigned short BondOrder;
  
  class Molecule;

  class Bond
  {
  public:
    void PrintSelf(ostream &os, vtkIndent indent);

    /**
     * Return the IdBond_ used to identify this bond in the parent molecule.
     */
    vtkIdType GetId() const { return this->IdBond_; }

    /**
     * Return the parent molecule of this bond.
     */
    Molecule &GetMolecule() { return this->molecule_; }

    ///@{
    /**
     * Get the starting / ending atom ids for this bond.
     */
    vtkIdType GetBeginAtomId() const;
    vtkIdType GetEndAtomId() const;
    ///@}

    ///@{
    /**
     * Get a Atom object that refers to the starting / ending atom
     * for this bond.
     */
    Atom GetBeginAtom();
    Atom GetEndAtom();
    Atom GetBeginAtom() const;
    Atom GetEndAtom() const;
    ///@}

    /**
     * Get the bond order for this bond.
     */
    BondOrder GetOrder();

    /**
     * Get the distance between the bonded atoms.

     * @note This function is faster than Molecule::GetBondLength and
     * should be used when possible.
     */
    double GetLength() const;

    vtkIdType GetSpotId() const;

  protected:
    friend class Molecule;

    explicit Bond(Molecule &parent, vtkIdType /*idBond*/,
     vtkIdType /* idAtomBegin */, vtkIdType /* idAtomEnd */);

    Molecule &molecule_;
    vtkIdType IdBond_ = -1;
    vtkIdType IdAtomBegin_ = -1;
    vtkIdType IdAtomEnd_ = -1;
    vtkIdType IdSpot_ = -1;
  };

}; // namespace vtk

#endif // !Bond_h