#include "AcquireMoleculeFile.h"
using namespace vtk;

#include "Molecule.h"

#include <vtkExecutive.h>
#include <vtkObjectFactory.h>

#include <vtkInformation.h>
#include <vtkInformationVector.h>

//------------------------------------------------------------------------------
vtkStandardNewMacro(AcquireMoleculeFile);

//------------------------------------------------------------------------------
AcquireMoleculeFile::AcquireMoleculeFile(int nInPorts, int nOutPorts)
    : MoleculeBuild(nOutPorts)
{
    this->SetNumberOfInputPorts(nInPorts);
}

//------------------------------------------------------------------------------
Molecule *AcquireMoleculeFile::GetOutput()
{
    return Molecule::SafeDownCast(this->GetOutputDataObject(0));
}

//------------------------------------------------------------------------------
void AcquireMoleculeFile::SetOutput(Molecule *output)
{
    this->GetExecutive()->SetOutputData(0, output);
}

//------------------------------------------------------------------------------
void AcquireMoleculeFile::PrintSelf(ostream &os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << indent << "[ FileName: " << this->GetFileName()
       << " Running number of atoms: " << this->GetNumberOfAtoms()
       << " Running title" << this->GetStructureName()
       << " ]" << endl;
}

//------------------------------------------------------------------------------
int AcquireMoleculeFile::ScrollStrings(std::istream &in, int ns)
{
    if (ns <= 0)
        return 0;
    std::string one;
    do
    {
        std::getline(in, one);
    } while (--ns);
    return ns;
}

//------------------------------------------------------------------------------
int AcquireMoleculeFile::RequestInformation(vtkInformation *vtkNotUsed(request),
                                vtkInformationVector **vtkNotUsed(inputVector),
                                vtkInformationVector *outputVector)
{
    vtkInformation *outInfo = outputVector->GetInformationObject(0);
    Molecule *output = Molecule::SafeDownCast(vtkDataObject::GetData(outputVector));

    if (!output)
    {
        vtkErrorMacro("AcquireMoleculeFile does not have a Molecule as output.");
        return 1;
    }

    InputFile fileInput(this->GetFileName());

    if (!fileInput.is_open())
    {
        vtkErrorMacro("AcquireMoleculeFile error opening file: " << this->GetFileName());
        return 0;
    }

    return this->CheckSizesOf(fileInput) ? 1 : 0; // virtually Ok
}

//------------------------------------------------------------------------------
int AcquireMoleculeFile::RequestData(vtkInformation *vtkNotUsed(request),
                                vtkInformationVector **vtkNotUsed(inputVector),
                                vtkInformationVector *outputVector)
{
    vtkInformation *outInfo = outputVector->GetInformationObject(0);
    Molecule *output = Molecule::SafeDownCast(vtkDataObject::GetData(outputVector));

    if (!output)
    {
        vtkErrorMacro("AcquireMoleculeFile does not have a Molecule as output.");
        return 1;
    }

    InputFile fileInput(this->GetFileName());

    if (!fileInput.is_open())
    {
        vtkErrorMacro("AcquireMoleculeFile error opening file: " << this->GetFileName());
        return 0;
    }

        return this->ReadDataFrom(fileInput) ? 1 : 0; // virtually Ok
}

int AcquireMoleculeFile::CheckSizesOf(InputFile& /*inp*/) { return 1; }
int AcquireMoleculeFile::ReadDataFrom(InputFile& /*inp*/) { return 1; }
