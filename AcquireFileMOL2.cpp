#include "AcquireFileMOL2.h"

//------------------------------------------------------------------------------
vtkStandardNewMacro(AcquireFileMOL2);

//------------------------------------------------------------------------------
AcquireFileMOL2::AcquireFileMOL2() : AcquireFileBase(1) {}

//------------------------------------------------------------------------------
void AcquireFileMOL2::PrintSelf(ostream &os, vtkIndent indent)
{
  os << indent << "MOL2 parser:\n";
  this->Superclass::PrintSelf(os, indent);
}

/*
//------------------------------------------------------------------------------
int AcquireFileMOL2::OnReadDataComplete(Molecule *ptrMol)
{
    if (!this->Superclass::OnReadDataComplete(ptrMol) )
     return 0;

    vtkNew<Molecule> newmol;

    // Let's make some possible bonds:
    vtkNew<MakeBondsDistances> mk_bonds;
    mk_bonds->SetInputData(this->GetOutput());
    mk_bonds->SetOutput(newmol);
    mk_bonds->Update();

    ptrMol->DeepCopy(newmol);
    return (ptrMol->GetNumberOfAtoms() > 0 ? 1 : 0);
}
*/