#ifndef Molecule_h__
#define Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    Molecule.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   Molecule
 * @brief   class describing a molecule
 *
 * Molecule and the convenience classes Atom and Bond
 * describe the geometry and connectivity of a molecule. The molecule
 * can be constructed using the AppendAtom() and AppendBond() methods in one
 * of two ways; either by fully specifying the atom/bond in a single
 * call, or by incrementally setting the various attributes using the
 * convenience Atom and Bond classes:
 *
 * Single call:
 * \code
 * Molecule *mol = Molecule::New();
 * Atom h1 = mol->AppendAtom(1, 0.0, 0.0, -0.5);
 * Atom h2 = mol->AppendAtom(1, 0.0, 0.0,  0.5);
 * Bond b  = mol->AppendBond(h1, h2, 1);
 * \endcode
 *
 * Incremental:
 * \code
 * Molecule *mol = Molecule::New();
 *
 * Atom h1 = mol->AppendAtom();
 * h1.SetAtomicNumber(1);
 * h1.SetPosition(0.0, 0.0, -0.5);
 *
 * Atom h2 = mol->AppendAtom();
 * h2.SetAtomicNumber(1);
 * vtkVector3d displacement (0.0, 0.0, 1.0);
 * h2.SetPosition(h1.GetPositionAsVector3d() + displacement);
 *
 * Bond b  = mol->AppendBond(h1, h2, 1);
 * \endcode
 *
 * Both of the above methods will produce the same molecule, two
 * hydrogens connected with a 1.0 Angstrom single bond, aligned to the
 * z-axis. The second example also demonstrates the use of VTK's
 * vtkVector class, which is fully supported by the Chemistry kit.
 *
 * The Molecule object is intended to be used with the
 * vtkMoleculeMapper class for visualizing molecular structure using
 * common rendering techniques.
 *
 * \warning While direct use of the underlying vtkUndirectedGraph
 * structure is possible due to Molecule's public inheritance, this
 * should not be relied upon and may change in the future.
 *
 * @sa
 * Atom Bond vtkMoleculeMapper vtkPeriodicTable
 */

// #include <vtkCommonDataModelModule.h> // For export macro
#include <vtkUndirectedGraph.h>

#include <vtkNew.h>          // For vtkNew
#include <vtkSmartPointer.h> // For vtkSmartPointer

#include "CriticalPoint.h"
#include "Atom.h" // Simple proxy class dependent on Molecule
#include "Bond.h" // Simple proxy class dependent on Molecule

#include <vtkVector.h> // Small templated vector convenience class

#include <string>

class vtkAbstractElectronicData;
class vtkDataArray;
class vtkInformation;
class vtkInformationVector;
class vtkMatrix3x3;
class vtkPlane;
class vtkPoints;
class vtkUnsignedCharArray;
class vtkUnsignedShortArray;
class vtkIdTypeArray;

namespace vtk
{
    typedef vtkUnsignedShortArray ArrayAtomTypes;
    typedef vtkUnsignedShortArray ArrayBondOrders;

    typedef vtkNew<vtkPoints> NewPoints;
    typedef vtkSmartPointer<vtkPoints> ThePoints;

    class /*VTKCOMMONDATAMODEL_EXPORT*/ Molecule
        : public vtkUndirectedGraph
    {
    public:
        static Molecule *New();
        vtkTypeMacro(Molecule, vtkUndirectedGraph);
        void PrintSelf(ostream & /* os */, vtkIndent /*indent*/) override;

        // Interface
        void Initialize() override;

        enum
        {
            Angstrom = 0,
            Bohr = 1
        };

        /**
         * Return what type of dataset this is.
         */
        int GetDataObjectType() override { return VTK_MOLECULE; }

        /**
         * Add new atom with atomic number 0 (dummy atom) at origin. Return
         * a Atom that refers to the new atom.
         */
        Atom AppendAtom() { return this->AppendAtom(0, 0., 0., 0.); }

        ///@{
        /**
         * Add new atom with the specified atomic number and position. Return a
         * Atom that refers to the new atom.
         */
        Atom AppendAtom(IdAtomType /* idAtomType */, double x, double y, double z);
        //
        Atom AppendAtom(IdAtomType idAtomType, const vtkVector3f &pos)
        {
            return this->AppendAtom(idAtomType, pos[0], pos[1], pos[2]);
        }

        Atom AppendAtom(IdAtomType idAtomType, double pos[3])
        {
            return this->AppendAtom(idAtomType, pos[0], pos[1], pos[2]);
        }
        Atom AppendAtom(IdAtomType /* idAtomType */,
                        const vtkVector3f & /* ZRef   = (length, angle, dihedral) */,
                        const vtkVector3i & /* ZFrame = (idLengthTo, idAngleWithL, idDihedralWithLA) */);
        ///@}

        /**
         * Return a Atom that refers to the atom with the specified id.
         */
        Atom GetAtom(vtkIdType atomId);

        /**
         * Return the number of atoms in the molecule.
         */
        vtkIdType GetNumberOfAtoms();

        ///@{
        /**
         * Add a bond between the specified atoms, optionally setting the
         * bond order (default: 1). Return a Bond object referring to the
         * new bond.
         */
        Bond AppendBond(vtkIdType atom1, vtkIdType atom2, unsigned short order = 1);
        Bond AppendBond(const Atom &atom1, const Atom &atom2, unsigned short order = 1)
        {
            return this->AppendBond(atom1.GetId(), atom2.GetId(), order);
        }
        ///@}

        /**
         * Return a Atom that refers to the bond with the specified id.
         */
        Bond GetBond(vtkIdType bondId);

        /**
         * Return the number of bonds in the molecule.
         */
        vtkIdType GetNumberOfBonds();

        /**
         * Return the atomic number of the atom with the specified id.
         */
        unsigned short GetAtomTypeId(vtkIdType atomId);

        /**
         * Set the atomic number of the atom with the specified id.
         */
        void SetAtomTypeId(vtkIdType atomId, unsigned short atomicNum, unsigned short kAux = 0);

        ///@{
        /**
         * Set the position of the atom with the specified id.
         */
        void SetAtomPosition(vtkIdType atomId, const vtkVector3f &pos);
        void SetAtomPosition(vtkIdType atomId, double x, double y, double z);
        void SetAtomPosition(vtkIdType atomId, double pos[3])
        {
            this->SetAtomPosition(atomId, pos[0], pos[1], pos[2]);
        }
        ///@}

        ///@{
        /**
         * Get the position of the atom with the specified id.
         */
        vtkVector3f GetAtomPosition(vtkIdType atomId);
        void GetAtomPosition(vtkIdType atomId, float pos[3]);
        void GetAtomPosition(vtkIdType atomId, double pos[3]);
        ///@}

        ///@{
        /**
         * Get/Set the bond order of the bond with the specified id
         */
        void SetBondOrder(vtkIdType bondId, BondOrder order);
        BondOrder GetBondOrder(vtkIdType bondId);
        ///@}

        /**
         * Get the bond length of the bond with the specified id

         * \note If the associated Bond object is already available,
         * Bond::GetBondLength is potentially much faster than this
         * function, as a list of all bonds may need to be constructed to
         * locate the appropriate bond.
         * \sa UpdateBondList()
         */
        double GetBondLength(vtkIdType bondId);

        ///@{
        /**
         * Set the position of the atom with the specified id.
         */
        // void SetBondSpot(vtkIdType atomId, const vtkVector3f &pos);
        // void SetBondSpot(vtkIdType atomId, double x, double y, double z);
        // void SetBondSpot(vtkIdType atomId, double pos[3])
        // {
        //    this->SetBondSpot(atomId, pos[0], pos[1], pos[2]);
        // }
        ///@}

        ///@{
        /**
         * Get the spot position of the bond with the specified id.
         */
        // vtkVector3f GetBondSpot(vtkIdType idBond);
        // void GetBondSpot(vtkIdType idBond, float pos[3]);
        // void GetBondSpot(vtkIdType idBond, double pos[3]);
        ///@}

        ///@{
        /**
         * Access the raw arrays used in this Molecule instance
         */
        vtkPoints *GetAtomicPositionArray();
        ArrayAtomTypes *GetAtomicNumberArray();
        //
        ArrayBondOrders *GetBondOrdersArray();
        ///@}

        ///@{
        /**
         * Set/Get the AbstractElectronicData-subclassed object for this molecule.
         */
        vtkGetObjectMacro(ElectronicData, vtkAbstractElectronicData);
        virtual void SetElectronicData(vtkAbstractElectronicData *);
        ///@}

        /**
         * Performs the same operation as ShallowCopy(),
         * but instead of reporting an error for an incompatible graph,
         * returns false.
         */
        bool CheckedShallowCopy(vtkGraph *g) override;

        /**
         * Performs the same operation as DeepCopy(),
         * but instead of reporting an error for an incompatible graph,
         * returns false.
         */
        bool CheckedDeepCopy(vtkGraph *g) override;

        /**
         * Shallow copies the data object into this molecule.
         */
        void ShallowCopy(vtkDataObject *obj) override;

        /**
         * Deep copies the data object into this molecule.
         */
        void DeepCopy(vtkDataObject *obj) override;

        /**
         * Shallow copies the atoms and bonds from @a m into @a this.
         */
        virtual void ShallowCopyStructure(Molecule *m);

        /**
         * Deep copies the atoms and bonds from @a m into @a this.
         */
        virtual void DeepCopyStructure(Molecule *m);

        /**
         * Shallow copies attributes (i.e. everything besides atoms and bonds) from
         * @a m into @a this.
         */
        virtual void ShallowCopyAttributes(Molecule *m);

        /**
         * Deep copies attributes (i.e. everything besides atoms and bonds) from
         * @a m into @a this.
         */
        virtual void DeepCopyAttributes(Molecule *m);

        ///@{
        /**
         * Obtain the plane that passes through the indicated bond with the given
         * normal. If the plane is set successfully, the function returns true.

         * If the normal is not orthogonal to the bond, a new normal will be
         * constructed in such a way that the plane will be orthogonal to
         * the plane spanned by the bond vector and the input normal vector.

         * This ensures that the plane passes through the bond, and the
         * normal is more of a "hint" indicating the orientation of the plane.

         * The new normal (n) is defined as the input normal vector (n_i) minus
         * the projection of itself (proj[n_i]_v) onto the bond vector (v):

         * @verbatim
         * v ^
         * |  n = (n_i - proj[n_j]_v)
         * proj[n_i]_v ^  |----x
         * |  |   /
         * |  |  / n_i
         * |  | /
         * |  |/
         * @endverbatim

         * If n_i is parallel to v, a warning will be printed and no plane will be
         * added. Obviously, n_i must not be parallel to v.
         */
        static bool GetPlaneFromBond(const Bond &bond, const vtkVector3f &normal, vtkPlane *plane);
        static bool GetPlaneFromBond(
            const Atom &atom1, const Atom &atom2, const vtkVector3f &normal, vtkPlane *plane);
        ///@}

        /**
         * @brief AddCriticalPoint
         *
         */

        vtkIdType AddCriticalPoint(CriticalPointType type, double x, double y, double z, vtkIdTypeArray *context = nullptr);

        /**
         * Get the array that defines the ghost type of each atom.
         */
        vtkUnsignedCharArray *GetAtomGhostArray();

        /**
         * Allocate ghost array for atoms.
         */
        void AllocateAtomGhostArray();

        /**
         * Get the array that defines the ghost type of each bond.
         */
        vtkUnsignedCharArray *GetBondGhostArray();

        /**
         * Allocate ghost array for bonds.
         */
        void AllocateBondGhostArray();

        /**
         * Initialize a molecule with an atom per input point.
         * Parameters atomPositions and atomicNumberArray should have the same size.
         */
        int Initialize(
            vtkPoints *atomPositions, vtkDataArray *atomicNumberArray, vtkDataSetAttributes *atomData);

        /**
         * Overloads Initialize method.
         */
        int Initialize(vtkPoints *atomPositions, vtkDataSetAttributes *atomData)
        {
            return this->Initialize(atomPositions, nullptr, atomData);
        }

        /**
         * Use input molecule points, atomic number and atomic data to initialize the new molecule.
         */
        int Initialize(Molecule *molecule);

        ///@{
        /**
         * Retrieve a molecule from an information vector.
         */
        static Molecule *GetData(vtkInformation *info);
        static Molecule *GetData(vtkInformationVector *v, int i = 0);
        ///@}

        /**
         * Return the VertexData of the underlying graph
         */
        vtkDataSetAttributes *GetAtomData() { return this->GetVertexData(); }

        /**
         * Return the EdgeData of the underlying graph
         */
        vtkDataSetAttributes *GetBondData() { return this->GetEdgeData(); }

        /**
         * Return the edge id from the underlying graph.
         */
        vtkIdType GetBondId(vtkIdType a, vtkIdType b) { return this->GetEdgeId(a, b); }

        /*virtual*/
        void SetAtomicNumberArrayName(const char *name) { AtomicNumberArrayName.assign(name); }
        /*virtual*/
        const char *GetAtomicNumberArrayName() const { return AtomicNumberArrayName.c_str(); }

        /*virtual*/
        void SetBondOrdersArrayName(const char *name) { BondOrdersArrayName.assign(name); }
        /*virtual*/
        const char *GetBondOrdersArrayName() const { return BondOrdersArrayName.c_str(); }

        /**
         * Return the actual size of the data in kibibytes (1024 bytes). This number
         * is valid only after the pipeline has updated. The memory size
         * returned is guaranteed to be greater than or equal to the
         * memory required to represent the data (e.g., extra space in
         * arrays, etc. are not included in the return value).
         */
        unsigned long GetActualMemorySize() override;

        short int isAtomicUnits() const { return idUnits_; }
        void resetUnits(short int /* units */);

    protected:
        Molecule();
        ~Molecule() override;

        short int idUnits_ = Angstrom;

        /**
         * Copy bonds and atoms.
         */
        virtual void CopyStructureInternal(Molecule *m, bool deep);

        /**
         * Copy everything but bonds and atoms.
         */
        virtual void CopyAttributesInternal(Molecule *m, bool deep);

        ///@{
        /**
         * The graph superclass does not provide fast random access to the
         * edge (bond) data. All random access is performed using a lookup
         * table that must be rebuilt periodically. These allow for lazy
         * building of the lookup table
         */
        void SetBondListDirty(bool bYes = true) { this->isBondListDirty_ = bYes; }
        bool IsBondListDirty() const { return this->isBondListDirty_; }
        void UpdateBondList();
        vtkIdTypeArray *GetBondList();
        ///@}

        friend class Atom;
        friend class Bond;

        vtkAbstractElectronicData *ElectronicData;

        vtkUnsignedCharArray *AtomGhostArray;
        vtkUnsignedCharArray *BondGhostArray;

        String AtomicNumberArrayName;
        String BondOrdersArrayName;

    private:
        bool isBondListDirty_ = false;

        Molecule(const Molecule &) = delete;
        void operator=(const Molecule &) = delete;
    };

}; // namespace vtk

#endif // !Molecule_h__
