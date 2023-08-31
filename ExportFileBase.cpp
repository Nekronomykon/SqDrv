#include "ExportFileBase.h"

using namespace std;

//------------------------------------------------------------------------------
vtkStandardNewMacro(ExportFileBase);

//------------------------------------------------------------------------------
ExportFileBase::ExportFileBase(int nOuts) : ResultsFromMolecule(nOuts)
{
}

//------------------------------------------------------------------------------
void ExportFileBase::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
