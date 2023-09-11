#include "AcquireFileMOL2.h"

#include "Elements.h"
using namespace vtk;

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
vtkIdType &AcquireFileMOL2::NumberOfBonds()
{
  return numberOfBonds_;
}

//------------------------------------------------------------------------------
vtkIdType &AcquireFileMOL2::NumberOfFragments()
{
  return numberOfFragments_;
}

//------------------------------------------------------------------------------
vtkIdType AcquireFileMOL2::GetNumberOfBonds() const
{
  return numberOfBonds_;
}

//------------------------------------------------------------------------------
vtkIdType AcquireFileMOL2::GetNumberOfFragments() const
{
  return numberOfFragments_;
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
  if (!ScrollToPrefix(inp, "@<TRIPOS>ATOM", one_line))
  {
    vtkErrorMacro("AcquireFileMOL2 error reading ATOM section from: " << this->getPath().string());
    return 0;
  }
  // reading atoms:
  for (vtkIdType i = 0; i < this->GetNumberOfAtoms(); i++)
  {
    /* code */
    if (!GetLine(inp, one_line))
    {
      vtkErrorMacro("AcquireFileMOL2 error reading ATOM line from: " << this->getPath().string());
      return 0;
    }
    InputString inps(one_line);
    String skip;
    String label;
    double x(0), y(0), z(0), q(0);
    int idFrag;
    String type;
    String nameFrag;
    inps >> skip              // atom ordinal that is not used
        >> label              // Atom name
        >> x >> y >> z        // Coords
        >> type               // SYBYL type
        >> idFrag >> nameFrag // Fragment IDs
        >> q                  // formal charge
        ;
    vtkIdType idElem = Elements::SymbolToNumber(type.c_str());
    if (!Elements::IsValidAtomNumber(idElem))
      idElem = Elements::SymbolToNumber(label.c_str());
    ptrMol->AppendAtom(idElem, x, y, z);
  }

  if (!ScrollToPrefix(inp, "@<TRIPOS>BOND", one_line))
  {
    vtkErrorMacro("AcquireFileMOL2 error reading ATOM section from: " << this->getPath().string());
    return 0;
  }
  // reading bonds:
  for (vtkIdType i = 0; i < this->GetNumberOfBonds(); i++)
  {
    /* code */
    if (!GetLine(inp, one_line))
    {
      vtkErrorMacro("AcquireFileMOL2 error reading BOND line from: " << this->getPath().string());
      return 0;
    }
    InputString inps(one_line);
    String skip;
    vtkIdType idFrom, idTo, idBond(0);
    String type;
    inps >> skip // bond ordinal that is not used
        >> idFrom >> idTo >> type;
    if (type[0] == '1')
      idBond = 1;
    else if (type[0] == '2')
      idBond = 2;
    else if (type[0] == '3')
      idBond = 3;
    else if (type[0] == 'a')
      idBond = 7; // aromatic, amide --> to think on it
    ptrMol->AppendBond(--idFrom,--idTo, idBond);
  }

  return (ptrMol->GetNumberOfAtoms() > 0)
             ? 1
             : 0;
}
