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

//------------------------------------------------------------------------------
// int AcquireFileWFN::RequestInformation(vtkInformation *vtkNotUsed(request),
//                                       vtkInformationVector **vtkNotUsed(inputVector),
//                                       vtkInformationVector *outputVector)
int AcquireFileMOL2::ReadSizesFrom(InputFile &inp)
{
  /*
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  */
  // call base class:
  if (!this->Superclass::ReadSizesFrom(inp))
    return 0;

  String one_line;
  if (!ScrollToPrefix(inp, "@<TRIPOS>MOLECULE", one_line))
  {
    vtkErrorMacro("AcquireFileMOL2 error reading MOLECULE section from: " << this->getPath().string());
    return 0;
  }

  if (!GetLine(inp, this->Title())) // first (title) line may be empty
  {
    vtkErrorMacro("AcquireFileMOL2 error reading title string from: " << this->getPath().string());
    return 0;
  }

  if (!GetLine(inp, one_line))
  {
    vtkErrorMacro("AcquireFileMOL2 error reading size string from: " << this->getPath().string());
    return 0;
  }

  String skip;
  InputString inps(one_line);
  // int nAtoms, nBonds, nStrs, nFeats, nSets;
  inps >> this->NumberOfAtoms() >> this->NumberOfBonds() >> this->NumberOfFragments() >> skip >> skip;

  return (this->GetNumberOfAtoms() > 0)
             ? 1
             : 0;
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

//------------------------------------------------------------------------------
int AcquireFileMOL2::ReadDataFrom(InputFile &inp, Molecule *ptrMol)
{
  // call base class:
  if (!this->Superclass::ReadDataFrom(inp, ptrMol))
    return 0;

  String one_line;

  return 1;
}