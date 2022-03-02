#ifndef Atom_h
#define Atom_h

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    Atom.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   Atom
 * @brief   convenience proxy for Molecule
 *
 */

#include "vtkObject.h"                // For macros, defines, etc

class vtkVector3d;
class vtkVector3f;

namespace vtk
{
    class Molecule;

    class Atom
    {
    public:
        void PrintSelf(ostream &os, vtkIndent indent);

        /**
         * Return the Id used to identify this atom in the parent molecule.
         */
        inline vtkIdType GetId() const
        {
            return this->Id;
        }

        /**
         * Return the parent molecule of this atom.
         */
        inline Molecule *GetMolecule()
        {
            return this->molecule_;
        }

        ///@{
        /**
         * Get/Set the atomic number of this atom
         */
        unsigned short GetAtomicNumber() const;
        void SetAtomicNumber(unsigned short atomicNum);
        ///@}

        ///@{
        /**
         * Get/Set the position of this atom
         */
        void GetPosition(float pos[3]) const;
        void GetPosition(double pos[3]) const;
        void SetPosition(const float pos[3]);
        void SetPosition(float x, float y, float z);
        vtkVector3f GetPosition() const;
        void SetPosition(const vtkVector3f &pos);
        ///@}

    protected:
        friend class Molecule;

        Atom(Molecule *parent, vtkIdType id);

        Molecule *molecule_;
        vtkIdType Id;
    };

};
    // namespace vtk

#endif // !Atom_h