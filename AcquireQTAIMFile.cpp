#include "AcquireQTAIMFile.h"

using namespace vtk;
//------------------------------------------------------------------------------
vtkStandardNewMacro(AcquireQTAIMFile);

//------------------------------------------------------------------------------
AcquireQTAIMFile::AcquireQTAIMFile()
    : AcquireMoleculeFile(2, 2)
{
}

//------------------------------------------------------------------------------
void AcquireQTAIMFile::PrintSelf(ostream &os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << endl
       << indent << " Criticals: ACP = " << this->GetNumberOfACP() << endl
       << indent << "          ( NACP " << this->GetNumberOfNACP()
       << "; NNACP " << this->GetNumberOfNNACP() << ")" << endl
       << indent
       << "          : BCP = " << this->GetNumberOfBCP() << endl
       << indent
       << "          : RCP = " << this->GetNumberOfRCP() << endl
       << indent
       << "          : CCP = " << this->GetNumberOfCCP() << endl
       << indent;
}
