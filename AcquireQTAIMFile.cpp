#include "AcquireQTAIMFile.h"

#include <vtkInformation.h>
#include <vtkInformationVector.h>

using namespace vtk;
//------------------------------------------------------------------------------
vtkStandardNewMacro(AcquireQTAIMFile);

//------------------------------------------------------------------------------
AcquireQTAIMFile::AcquireQTAIMFile() : AcquireMoleculeFile(0, 1) {}
// temporary; nOuts should be greater than 1 -->
// do we need ports for the QTAIM results? Or the Molecule should store some
// QTAIM info within itself?

//------------------------------------------------------------------------------
void AcquireQTAIMFile::PrintSelf(ostream &os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << endl
       << indent << " Criticals: Total " << this->GetNumberOfCP() << endl
       << indent << "          : ACP = " << this->GetNumberOfACP() << endl
       << indent << "          ( NACP " << this->GetNumberOfNACP() << "; NNACP " << this->GetNumberOfNNACP() << ")" << endl
       << indent << "          : BCP = " << this->GetNumberOfBCP() << endl
       << indent << "          : RCP = " << this->GetNumberOfRCP() << endl
       << indent << "          : CCP = " << this->GetNumberOfCCP() << endl
        // << indent
        ;
}

// To be overriden to read information stored in the (file) stream
// int AcquireQTAIMFile::RequestInformation(vtkInformation *p_info, vtkInformationVector **pp_ifv, vtkInformationVector *p_ifv)
int AcquireQTAIMFile::ReadSizesFrom(InputFile &inp)
{
    // call base class:
    if (!this->Superclass::ReadSizesFrom(inp))
        return 0;


    // whatever
    return (this->ReadQTAIMSizes(inp)) ? 1 : 0;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////
// To be overriden to read information stored in the (file) stream
// int AcquireQTAIMFile::RequestData(vtkInformation *p_info, vtkInformationVector **pp_ifv, vtkInformationVector *p_ifv)
//
int AcquireQTAIMFile::ReadDataFrom(InputFile& inp)
{
    // call base class:
    if (!this->Superclass::RequestData(inp))
        return 0;
    // this->GetOutput()->InitCriticalData(this->GetNumberOfCP());

    std::string one_line;
    vtkIdType nReadCrit = 0;
    // ??  inp.seekg(0L);
    /*
    if(!GetLinePrefixed(inp, "CP#", one_line))
    return 0;
    do
    {
        
    } while (GetLinePrefixed(inp, "CP#", one_line));
    */
    // whatever else...
    return 1;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////
//
int AcquireQTAIMFile::ReadQTAIMSizes(InputFile& /* inp */)
{
    NumberOfNACP_ = this->GetNumberOfAtoms(); 
    return (NumberOfNACP_) ? 1 : 0;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////
//
