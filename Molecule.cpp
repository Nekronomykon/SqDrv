#include "Molecule.h"
using namespace vtk;

/*=========================================================================

Program:   Visualization Toolkit
Module:    Molecule.cxx
Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <vtkAbstractElectronicData.h>
#include <vtkDataSetAttributes.h>
#include <vtkEdgeListIterator.h>
#include <vtkFloatArray.h>
#include <vtkGraphInternals.h>
#include <vtkIdTypeArray.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkMatrix3x3.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkPlane.h>
#include <vtkPoints.h>
#include <vtkUnsignedCharArray.h>
#include <vtkUnsignedShortArray.h>
#include <vtkVector.h>
#include <vtkVectorOperators.h>

#include "Elements.h"
using namespace vtk;

#include <cassert>

//------------------------------------------------------------------------------
vtkStandardNewMacro(Molecule);

//------------------------------------------------------------------------------
Molecule::Molecule()
    : ElectronicData(nullptr), AtomGhostArray(nullptr), BondGhostArray(nullptr)
//, AtomicNumberArrayName(nullptr)
// , BondOrdersArrayName(nullptr)
{
    this->Initialize();
}

//------------------------------------------------------------------------------
void Molecule::Initialize()
{
    // Reset underlying data structure
    this->Superclass::Initialize();

    // Setup vertex data
    vtkDataSetAttributes *vertData = this->GetVertexData();
    vertData->AllocateArrays(1); // atomic nums

    // Atomic numbers
    this->SetAtomicNumberArrayName("Atomic Numbers");
    vtkNew<ArrayAtomTypes> atomicNums;
    atomicNums->SetNumberOfComponents(1);
    atomicNums->SetName(this->GetAtomicNumberArrayName());
    vertData->SetScalars(atomicNums);

    // Nuclear coordinates
    vtkPoints *points = vtkPoints::New();
    this->SetPoints(points);
    points->Delete();

    // Setup edge data
    vtkDataSetAttributes *edgeData = this->GetEdgeData();
    edgeData->AllocateArrays(1); // Bond orders

    this->SetBondOrdersArrayName("Bond Orders");
    vtkNew<ArrayBondOrders> bondOrders;
    bondOrders->SetNumberOfComponents(1);
    bondOrders->SetName(this->GetBondOrdersArrayName());
    edgeData->SetScalars(bondOrders);

    this->UpdateBondList();

    // Electronic data
    this->SetElectronicData(nullptr);

    this->Modified();
}

//------------------------------------------------------------------------------
Molecule::~Molecule()
{
    this->SetElectronicData(nullptr);
    // delete[] this->AtomicNumberArrayName;
    // delete[] this->BondOrdersArrayName;
}

//------------------------------------------------------------------------------
void Molecule::PrintSelf(ostream &os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    vtkIndent subIndent = indent.GetNextIndent();

    os << indent << "Atoms:\n";
    for (vtkIdType i = 0; i < this->GetNumberOfAtoms(); ++i)
    {
        this->GetAtom(i).PrintSelf(os, subIndent);
    }

    os << indent << "Bonds:\n";
    for (vtkIdType i = 0; i < this->GetNumberOfBonds(); ++i)
    {
        os << subIndent << "===== Bond " << i << ": =====\n";
        this->GetBond(i).PrintSelf(os, subIndent);
    }

    os << indent << "Electronic Data:\n";
    if (this->ElectronicData)
    {
        this->ElectronicData->PrintSelf(os, subIndent);
    }
    else
    {
        os << subIndent << "Not set.\n";
    }

    os << indent << "Atomic number array name : " << this->GetAtomicNumberArrayName() << "\n";
    os << indent << "Bond orders array name : " << this->GetBondOrdersArrayName();
}

//------------------------------------------------------------------------------
Atom Molecule::AppendAtom(IdAtomType idAtomType, double x, double y, double z)
{
    ArrayAtomTypes *atomicNums = this->GetAtomicNumberArray();

    assert(atomicNums);

    vtkIdType id;
    this->AddVertexInternal(nullptr, &id);

    atomicNums->InsertValue(id, idAtomType);
    vtkIdType coordID = this->Points->InsertNextPoint(x, y, z);
    (void)coordID;
    assert("point ids synced with vertex ids" && coordID == id);

    this->Modified();
    return Atom(*this, id);
}

//------------------------------------------------------------------------------
Atom Molecule::AppendAtom(IdAtomType idAtomType, const vtkVector3f &ZRef, const vtkVector3i &ZFrame)
{
    assert((ZFrame.GetX() != ZFrame.GetY()) &&
           (ZFrame.GetY() != ZFrame.GetZ()) &&
           (ZFrame.GetZ() != ZFrame.GetX()));
    Atom aRes = this->AppendAtom(idAtomType, this->GetAtomPosition(ZFrame.GetX()));
    vtkVector3f xyz; // delta
    // NOT YET DONE!!!
    return aRes;
}

//------------------------------------------------------------------------------
Atom Molecule::GetAtom(vtkIdType atomId)
{
    assert(atomId >= 0 && atomId < this->GetNumberOfAtoms());

    Atom atom(*this, atomId);
    return atom;
}

//------------------------------------------------------------------------------
unsigned short Molecule::GetAtomTypeId(vtkIdType id)
{
    assert(id >= 0 && id < this->GetNumberOfAtoms());

    ArrayAtomTypes *atomicNums = this->GetAtomicNumberArray();

    return atomicNums->GetValue(id) & Elements::idAtomNumberMask;
}

//------------------------------------------------------------------------------
void Molecule::SetAtomTypeId(vtkIdType id, IndexElement atomicNum, unsigned short kAux)
{
    assert(id >= 0 && id < this->GetNumberOfAtoms());

    ArrayAtomTypes *atomicNums = this->GetAtomicNumberArray();

    atomicNums->SetValue(id, atomicNum);
    this->Modified();
}

//------------------------------------------------------------------------------
void Molecule::SetAtomPosition(vtkIdType id, const vtkVector3f &pos)
{
    assert(id >= 0 && id < this->GetNumberOfAtoms());
    this->Points->SetPoint(id, pos.GetData());
    this->Modified();
}

//------------------------------------------------------------------------------
void Molecule::SetAtomPosition(vtkIdType id, double x, double y, double z)
{
    assert(id >= 0 && id < this->GetNumberOfAtoms());
    this->Points->SetPoint(id, x, y, z);
    this->Modified();
}

//------------------------------------------------------------------------------
vtkVector3f Molecule::GetAtomPosition(vtkIdType id)
{
    assert(id >= 0 && id < this->GetNumberOfAtoms());
    vtkDataArray *positions = this->Points->GetData();
    auto positionsF = vtkArrayDownCast<vtkFloatArray>(positions);
    if (positionsF)
    {
        float *data = positionsF->GetPointer(id * 3);
        return vtkVector3f(data);
    }

    auto point = positions->GetTuple3(id);
    return vtkVector3f(point[0], point[1], point[2]);
}

//------------------------------------------------------------------------------
void Molecule::GetAtomPosition(vtkIdType id, float pos[3])
{
    vtkVector3f position = this->GetAtomPosition(id);
    pos[0] = position.GetX();
    pos[1] = position.GetY();
    pos[2] = position.GetZ();
}

//------------------------------------------------------------------------------
void Molecule::GetAtomPosition(vtkIdType id, double pos[3])
{
    this->Points->GetPoint(id, pos);
}

//------------------------------------------------------------------------------
vtkIdType Molecule::GetNumberOfAtoms()
{
    return this->GetNumberOfVertices();
}

//------------------------------------------------------------------------------
Bond Molecule::AppendBond(const vtkIdType atom1, const vtkIdType atom2, const unsigned short order)
{
    ArrayBondOrders *bondOrders = this->GetBondOrdersArray();

    assert(bondOrders);

    vtkEdgeType edgeType;
    this->AddEdgeInternal(atom1, atom2, false, nullptr, &edgeType);
    this->SetBondListDirty();

    vtkIdType id = edgeType.Id;
    bondOrders->InsertValue(id, order);
    this->Modified();
    return Bond(*this, id, atom1, atom2);
}

//------------------------------------------------------------------------------
Bond Molecule::GetBond(vtkIdType bondId)
{
    assert(bondId >= 0 && bondId < this->GetNumberOfBonds());

    vtkIdTypeArray *bonds = this->GetBondList();
    // An array with two components holding the bonded atom's ids
    vtkIdType *ids = bonds->GetPointer(2 * bondId);
    return Bond(*this, bondId, ids[0], ids[1]);
}

//------------------------------------------------------------------------------
void Molecule::SetBondOrder(vtkIdType bondId, BondOrder order)
{
    assert(bondId >= 0 && bondId < this->GetNumberOfBonds());

    ArrayBondOrders *bondOrders = this->GetBondOrdersArray();
    assert(bondOrders);

    /* bool bRes = */
    bondOrders->InsertValue(bondId, order);
    this->Modified(/* bRes */);
}

//------------------------------------------------------------------------------
BondOrder Molecule::GetBondOrder(vtkIdType bondId)
{
    assert(bondId >= 0 && bondId < this->GetNumberOfBonds());

    ArrayBondOrders *bondOrders = this->GetBondOrdersArray();

    return bondOrders ? bondOrders->GetValue(bondId) : 0;
}

//------------------------------------------------------------------------------
double Molecule::GetBondLength(vtkIdType bondId)
{
    Bond bond = this->GetBond(bondId);
    return bond.GetLength();
}

//------------------------------------------------------------------------------
vtkPoints *Molecule::GetAtomicPositionArray()
{
    return this->Points;
}

//------------------------------------------------------------------------------
ArrayAtomTypes *Molecule::GetAtomicNumberArray()
{
    ArrayAtomTypes *atomicNums = vtkArrayDownCast<ArrayAtomTypes>(
        this->GetVertexData()->GetScalars(this->GetAtomicNumberArrayName()));

    assert(atomicNums);

    return atomicNums;
}

//------------------------------------------------------------------------------
ArrayBondOrders *Molecule::GetBondOrdersArray()
{
    return vtkArrayDownCast<ArrayBondOrders>(
        this->GetBondData()->GetScalars(this->GetBondOrdersArrayName()));
}

//------------------------------------------------------------------------------
vtkIdType Molecule::GetNumberOfBonds()
{
    return this->GetNumberOfEdges();
}

//------------------------------------------------------------------------------
vtkCxxSetObjectMacro(Molecule, ElectronicData, vtkAbstractElectronicData);

//------------------------------------------------------------------------------
void Molecule::ShallowCopy(vtkDataObject *obj)
{
    Molecule *m = Molecule::SafeDownCast(obj);
    if (!m)
    {
        vtkErrorMacro("Can only shallow copy from Molecule or subclass.");
        return;
    }
    this->ShallowCopyStructure(m);
    this->ShallowCopyAttributes(m);
}

//------------------------------------------------------------------------------
void Molecule::DeepCopy(vtkDataObject *obj)
{
    Molecule *m = Molecule::SafeDownCast(obj);
    if (!m)
    {
        vtkErrorMacro("Can only deep copy from Molecule or subclass.");
        return;
    }
    this->DeepCopyStructure(m);
    this->DeepCopyAttributes(m);
}

//------------------------------------------------------------------------------
bool Molecule::CheckedShallowCopy(vtkGraph *g)
{
    bool result = this->Superclass::CheckedShallowCopy(g);
    this->SetBondListDirty(true);
    return result;
}

//------------------------------------------------------------------------------
bool Molecule::CheckedDeepCopy(vtkGraph *g)
{
    bool result = this->Superclass::CheckedDeepCopy(g);
    this->SetBondListDirty(true);
    return result;
}

//------------------------------------------------------------------------------
void Molecule::ShallowCopyStructure(Molecule *m)
{
    this->CopyStructureInternal(m, false);
}

//------------------------------------------------------------------------------
void Molecule::DeepCopyStructure(Molecule *m)
{
    this->CopyStructureInternal(m, true);
}

//------------------------------------------------------------------------------
void Molecule::ShallowCopyAttributes(Molecule *m)
{
    this->CopyAttributesInternal(m, false);
}

//------------------------------------------------------------------------------
void Molecule::DeepCopyAttributes(Molecule *m)
{
    this->CopyAttributesInternal(m, true);
}

//------------------------------------------------------------------------------
void Molecule::CopyStructureInternal(Molecule *m, bool deep)
{
    // Call superclass
    if (deep)
    {
        this->Superclass::DeepCopy(m);
    }
    else
    {
        this->Superclass::ShallowCopy(m);
    }

    this->SetBondListDirty(true);
}

//------------------------------------------------------------------------------
void Molecule::CopyAttributesInternal(Molecule *m, bool deep)
{
    if (deep)
    {
        if (m->ElectronicData)
            this->ElectronicData->DeepCopy(m->ElectronicData);
    }
    else
    {
        this->SetElectronicData(m->ElectronicData);
    }
}

//------------------------------------------------------------------------------
void Molecule::UpdateBondList()
{
    this->BuildEdgeList();
    this->SetBondListDirty(false);
}

//------------------------------------------------------------------------------
vtkIdTypeArray *Molecule::GetBondList()
{
    // Create the edge list if it doesn't exist, or is marked as dirty.
    vtkIdTypeArray *edgeList = this->IsBondListDirty() ? nullptr : this->GetEdgeList();
    if (!edgeList)
    {
        this->UpdateBondList();
        edgeList = this->GetEdgeList();
    }
    assert(edgeList != nullptr);
    return edgeList;
}

//------------------------------------------------------------------------------
bool Molecule::GetPlaneFromBond(const Bond &bond, const vtkVector3f &normal, vtkPlane *plane)
{
    return Molecule::GetPlaneFromBond(bond.GetBeginAtom(), bond.GetEndAtom(), normal, plane);
}

//------------------------------------------------------------------------------
bool Molecule::GetPlaneFromBond(
    const Atom &atom1, const Atom &atom2, const vtkVector3f &normal, vtkPlane *plane)
{
    if (!plane)
        return false;

    vtkVector3f v(atom1.GetPosition() - atom2.GetPosition());

    vtkVector3f n_i(normal);
    vtkVector3f unitV(v.Normalized());

    // Check if vectors are (nearly) parallel
    if (unitV.Compare(n_i.Normalized(), 1e-7))
        return false;

    // calculate projection of n_i onto v
    // TODO Remove or restore this when scalar mult. is supported again
    // vtkVector3d proj (unitV * n_i.Dot(unitV));
    double n_iDotUnitV = n_i.Dot(unitV);
    vtkVector3f proj(unitV[0] * n_iDotUnitV, unitV[1] * n_iDotUnitV, unitV[2] * n_iDotUnitV);
    // end vtkVector reimplementation TODO

    // Calculate actual normal:
    vtkVector3f realNormal(n_i - proj);

    // Create plane:
    vtkVector3f pos(atom1.GetPosition());
    plane->SetOrigin(pos.Cast<double>().GetData());
    plane->SetNormal(realNormal.Cast<double>().GetData());
    return true;
}

//------------------------------------------------------------------------------
vtkUnsignedCharArray *Molecule::GetAtomGhostArray()
{
    return vtkArrayDownCast<vtkUnsignedCharArray>(
        this->GetVertexData()->GetArray(vtkDataSetAttributes::GhostArrayName()));
}

//------------------------------------------------------------------------------
void Molecule::AllocateAtomGhostArray()
{
    if (this->GetAtomGhostArray() == nullptr)
    {
        vtkNew<vtkUnsignedCharArray> ghosts;
        ghosts->SetName(vtkDataSetAttributes::GhostArrayName());
        ghosts->SetNumberOfComponents(1);
        ghosts->SetNumberOfTuples(this->GetNumberOfAtoms());
        ghosts->FillComponent(0, 0);
        this->GetVertexData()->AddArray(ghosts);
    }
    else
    {
        this->GetAtomGhostArray()->SetNumberOfTuples(this->GetNumberOfAtoms());
    }
}

//------------------------------------------------------------------------------
vtkUnsignedCharArray *Molecule::GetBondGhostArray()
{
    return vtkArrayDownCast<vtkUnsignedCharArray>(
        this->GetEdgeData()->GetArray(vtkDataSetAttributes::GhostArrayName()));
}

//------------------------------------------------------------------------------
void Molecule::AllocateBondGhostArray()
{
    if (this->GetBondGhostArray() == nullptr)
    {
        vtkNew<vtkUnsignedCharArray> ghosts;
        ghosts->SetName(vtkDataSetAttributes::GhostArrayName());
        ghosts->SetNumberOfComponents(1);
        ghosts->SetNumberOfTuples(this->GetNumberOfBonds());
        ghosts->FillComponent(0, 0);
        this->GetEdgeData()->AddArray(ghosts);
    }
    else
    {
        this->GetBondGhostArray()->SetNumberOfTuples(this->GetNumberOfBonds());
    }
}

//------------------------------------------------------------------------------
int Molecule::Initialize(vtkPoints *atomPositions,
                         vtkDataArray *atomicNumberArray,
                         vtkDataSetAttributes *atomData)
{
    // Start with default initialization the molecule
    this->Initialize();

    // if no atomicNumberArray given, look for one in atomData
    if (!atomicNumberArray && atomData)
    {
        atomicNumberArray = atomData->GetArray(this->GetAtomicNumberArrayName());
    }

    if (!atomPositions && !atomicNumberArray)
    {
        vtkDebugMacro(<< "Atom position and atomic numbers were not found: skip atomic data.");
        return 1;
    }

    if (!atomPositions || !atomicNumberArray)
    {
        vtkDebugMacro(<< "Empty atoms or atomic numbers.");
        return 0;
    }

    // ensure it is a short array
    vtkNew<ArrayAtomTypes> newAtomicNumberShortArray;
    if (ArrayAtomTypes::SafeDownCast(atomicNumberArray))
    {
        newAtomicNumberShortArray->ShallowCopy(atomicNumberArray);
    }
    else
    {
        vtkIdType nbPoints = atomicNumberArray->GetNumberOfTuples();
        newAtomicNumberShortArray->SetNumberOfComponents(1);
        newAtomicNumberShortArray->SetNumberOfTuples(nbPoints);
        newAtomicNumberShortArray->SetName(atomicNumberArray->GetName());
        for (vtkIdType i = 0; i < nbPoints; i++)
        {
            newAtomicNumberShortArray->SetTuple1(i, atomicNumberArray->GetTuple1(i));
        }
    }

    int nbAtoms = atomPositions->GetNumberOfPoints();
    if (nbAtoms != newAtomicNumberShortArray->GetNumberOfTuples())
    {
        vtkErrorMacro(<< "Number of atoms not equal to number of atomic numbers.");
        return 0;
    }
    if (atomData && nbAtoms != atomData->GetNumberOfTuples())
    {
        vtkErrorMacro(<< "Number of atoms not equal to number of atom properties.");
        return 0;
    }

    static const std::string atomicNumberName = this->GetAtomicNumberArrayName();

    // update atoms positions
    this->ForceOwnership();
    this->Internals->Adjacency.resize(nbAtoms, vtkVertexAdjacencyList());
    this->SetPoints(atomPositions);

    // if atom properties exists, copy it in VertexData and look for an atomic number in its arrays.
    if (atomData)
    {
        this->GetVertexData()->ShallowCopy(atomData);

        // if atomData contains an array with the atomic number name,
        // copy this array with a new name as we will overwrite it.
        vtkDataArray *otherArray = atomData->GetArray(atomicNumberName.c_str());
        if (otherArray && otherArray != static_cast<vtkAbstractArray *>(atomicNumberArray))
        {
            this->GetVertexData()->RemoveArray(atomicNumberName.c_str());

            // create a new name for the copied array.
            std::string newName = std::string("Original ") + atomicNumberName;

            // if the new name is available create a copy of the array with another name, and add it
            // else no backup is done, array will be replaced.
            if (!atomData->GetArray(newName.c_str()))
            {
                vtkDataArray *otherArrayCopy = otherArray->NewInstance();
                otherArrayCopy->ShallowCopy(otherArray);
                otherArrayCopy->SetName(newName.c_str());
                this->GetVertexData()->AddArray(otherArrayCopy);
                otherArrayCopy->Delete();
            }
            else
            {
                vtkWarningMacro(<< "Array '" << atomicNumberName << "' was replaced.");
            }
        }
    }

    // add atomic number array: if given array has the expected name, add it directly
    // (it will replace the current one). Else copy it and add it with atomic number name.
    if (atomicNumberName == newAtomicNumberShortArray->GetName())
    {
        this->GetVertexData()->AddArray(newAtomicNumberShortArray);
    }
    else
    {
        vtkNew<ArrayAtomTypes> atomicNumberArrayCopy;
        atomicNumberArrayCopy->ShallowCopy(newAtomicNumberShortArray);
        atomicNumberArrayCopy->SetName(atomicNumberName.c_str());
        this->GetVertexData()->AddArray(atomicNumberArrayCopy.Get());
    }

    this->Modified();
    return 1;
}

//------------------------------------------------------------------------------
int Molecule::Initialize(Molecule *molecule)
{
    if (molecule == nullptr)
    {
        this->Initialize();
        return 1;
    }

    return this->Initialize(molecule->GetPoints(),
                            molecule->GetAtomicNumberArray(),
                            molecule->GetVertexData());
}

//------------------------------------------------------------------------------
Molecule *Molecule::GetData(vtkInformation *info)
{
    return info ? Molecule::SafeDownCast(info->Get(DATA_OBJECT())) : nullptr;
}

//------------------------------------------------------------------------------
Molecule *Molecule::GetData(vtkInformationVector *v, int i)
{
    return Molecule::GetData(v->GetInformationObject(i));
}

//------------------------------------------------------------------------------
unsigned long Molecule::GetActualMemorySize()
{
    unsigned long size = this->Superclass::GetActualMemorySize();
    if (this->ElectronicData)
    {
        size += this->ElectronicData->GetActualMemorySize();
    }
    if (this->AtomGhostArray)
    {
        size += this->AtomGhostArray->GetActualMemorySize();
    }
    if (this->BondGhostArray)
    {
        size += this->BondGhostArray->GetActualMemorySize();
    }
    return size;
}

//------------------------------------------------------------------------------
vtkIdType Molecule::AddCriticalPoint(CriticalPointType type, double x, double y, double z, vtkIdTypeArray *context)
{
    vtkIdType idCP(-1L);
    if (type.real() == 3) // only full-rank CP are allowed...
    {
        switch (type.imag())
        {
        case FullRankTypeMaximum:
            break;

        case FullRankTypeSaddleB:
            break;

        case FullRankTypeSaddleR:
            break;

        case FullRankTypeMinimum:
            break;

        default:
            break;
        }
    }
    return idCP;
}

//------------------------------------------------------------------------------
void Molecule::resetUnits(short int units)
{
    if (units != idUnits_)
    {
        // onChangedUnits --> scale coordinates?
    }
    idUnits_ = (!units) ? Angstrom : Bohr;
}
