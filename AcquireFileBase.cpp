#include "AcquireFileBase.h"

using namespace std;

//------------------------------------------------------------------------------
vtkStandardNewMacro(AcquireFileBase);

//------------------------------------------------------------------------------
AcquireFileBase::AcquireFileBase(int nOuts) : MoleculeSource(nOuts)
{
}

//------------------------------------------------------------------------------
void AcquireFileBase::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
