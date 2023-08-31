#include "AcquireFileBase.h"

#include <vtkExecutive.h>
#include <vtkObjectFactory.h>

#include <vtkInformation.h>
#include <vtkInformationVector.h>

using namespace std;

//------------------------------------------------------------------------------
vtkStandardNewMacro(AcquireFileBase);

//------------------------------------------------------------------------------
AcquireFileBase::AcquireFileBase(int nOuts) : SourceOfMolecule(nOuts)
{
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

//------------------------------------------------------------------------------
String AcquireFileBase::getNameOfStructure() const
{
  return nameStructure_;
}

//------------------------------------------------------------------------------
String AcquireFileBase::resetNameOfStructure(String name)
{
  std::swap(name, nameStructure_);
  return name;
}

//------------------------------------------------------------------------------
String &AcquireFileBase::NameOfStructure()
{
  return nameStructure_;
}

//------------------------------------------------------------------------------
void AcquireFileBase::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "[ FileName: " << this->getPath()
     << " Running number of atoms: " << this->GetNumberOfAtoms()
     << " Running title" << this->getNameOfStructure()
     << " ]" << endl;
}

//------------------------------------------------------------------------------
int AcquireFileBase::RequestInformation(vtkInformation *vtkNotUsed(request),
                                        vtkInformationVector **vtkNotUsed(inputVector),
                                        vtkInformationVector *outputVector)
{
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  Molecule *ptrMol = Molecule::SafeDownCast(vtkDataObject::GetData(outputVector));

  if (!ptrMol)
  {
    vtkErrorMacro("AcquireFileBase does not have a Molecule as ptrMol.");
    return 1;
  }

  InputFile fileInput(this->getPath());

  if (!fileInput.is_open())
  {
    vtkErrorMacro("AcquireFileBase error opening file: " << this->getPath().string());
    return 0;
  }

  int nRes = this->ReadSizesFrom(fileInput) ? 1 : 0; // virtually Ok
  fileInput.close();
  return nRes;
}

//------------------------------------------------------------------------------
int AcquireFileBase::RequestData(vtkInformation *vtkNotUsed(request),
                                 vtkInformationVector **vtkNotUsed(inputVector),
                                 vtkInformationVector *outputVector)
{
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  Molecule *ptrMol = Molecule::SafeDownCast(vtkDataObject::GetData(outputVector));

  if (!ptrMol)
  {
    vtkErrorMacro("AcquireFileBase does not have a Molecule as output");
    return 1;
  }

  InputFile fileInput(this->getPath());

  if (!fileInput.is_open())
  {
    vtkErrorMacro("AcquireFileBase error opening file: " << this->getPath().string());
    return 0;
  }

  int nRes = this->ReadDataFrom(fileInput, ptrMol) ? 1 : 0; // virtually Ok

  fileInput.close();

  if (nRes)
    nRes = this->OnReadDataComplete(ptrMol);

  return nRes;
}

int AcquireFileBase::ReadSizesFrom(InputFile & /*inp*/) { return 1; }
int AcquireFileBase::OnReadDataComplete(Molecule *ptrMol) { return ptrMol->GetNumberOfAtoms() > 0 ? 1 : 0; }
int AcquireFileBase::ReadDataFrom(InputFile & /*inp*/, Molecule *ptrMol)
{
  ptrMol->Initialize();
  return 1;
}
