#include "AcquireMoleculeFile.h"
using namespace vtk;

#include "Molecule.h"

#include <vtkExecutive.h>
#include <vtkObjectFactory.h>

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