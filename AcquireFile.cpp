#include "AcquireFile.h"

#include "Molecule.h"

using namespace vtk;

#include <vtkExecutive.h>
#include <vtkObjectFactory.h>

//------------------------------------------------------------------------------
vtkStandardNewMacro(AcquireFile);

//------------------------------------------------------------------------------
AcquireFile::AcquireFile(int nInPorts)
{
    this->SetNumberOfInputPorts(nInPorts);
}

//------------------------------------------------------------------------------
vtkMolecule *AcquireFile::GetOutput()
{
    return Molecule::SafeDownCast(this->GetOutputDataObject(0));
}

//------------------------------------------------------------------------------
void AcquireFile::SetOutput(vtkMolecule *output)
{
    this->GetExecutive()->SetOutputData(0, output);
}

//------------------------------------------------------------------------------
void AcquireFile::PrintSelf(ostream &os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << indent << "[ FileName: " << this->GetFileName()
       << " Running number of atoms: " << this->GetNumberOfAtoms()
       << " Running title" << this->GetStructureName()
       << " ]" << endl;
}

//------------------------------------------------------------------------------
int AcquireFile::ScrollStrings(std::istream &in, int ns)
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