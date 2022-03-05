#ifndef Bond_h
#define Bond_h
/*=========================================================================

  Program:   Visualization Toolkit
  Module:    Bond.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

/**
 * @class   Bond
 * @brief   convenience proxy for Molecule
 *
 */

#include "Atom.h"                     // For Atom
// #include "vtkCommonDataModelModule.h" // For export macro
#include <vtkObject.h>                // For macros, etc

namespace vtk
{
  class Molecule;

  class Bond
  {
  public:
    void PrintSelf(ostream &os, vtkIndent indent);

    /**
     * Return the Id used to identify this bond in the parent molecule.
     */
    vtkIdType GetId() const { return this->Id; }

    /**
     * Return the parent molecule of this bond.
     */
    Molecule *GetMolecule() { return this->molecule_; }

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
    unsigned short GetOrder();

    /**
     * Get the distance between the bonded atoms.

     * @note This function is faster than Molecule::GetBondLength and
     * should be used when possible.
     */
    double GetLength() const;

  protected:
    friend class Molecule;

    Bond(Molecule *parent, vtkIdType id, vtkIdType beginAtomId, vtkIdType endAtomId);

    Molecule *molecule_;
    vtkIdType Id;
    vtkIdType BeginAtomId;
    vtkIdType EndAtomId;
  };

}; // namespace vtk

#endif // !Bond_h