#include "AcquireFileBase.h"

using namespace std;

//------------------------------------------------------------------------------
vtkStandardNewMacro(AcquireFileBase);

//------------------------------------------------------------------------------
AcquireFileBase::AcquireFileBase(int nOuts) : SourceOfMolecule(nOuts)
{
}

//------------------------------------------------------------------------------
void AcquireFileBase::PrintSelf(ostream &os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
vtkIdType AcquireFileBase::GetNumberOfAtoms() const
{
    return NumberOfAtoms_;
}

//------------------------------------------------------------------------------
vtkIdType &AcquireFileBase::NumberOfAtoms()
{
    return NumberOfAtoms_;
}

//------------------------------------------------------------------------------
vtkIdType AcquireFileBase::resetNumberOfAtoms(vtkIdType id)
{
    std::swap(id, NumberOfAtoms_);
    return id;
}
