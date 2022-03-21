#include "AcquireQTAIMFile.h"

#include "Elements.h"
#include "Molecule.h"

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
    const char sKeyTotal[] = "Total number of electron density critical points found =";
    const char sKeyNACP[] = "Number of NACPs  =";
    const char sKeyNNACP[] = "Number of NNACPs =";
    const char sKeyBCP[] = "Number of BCPs   =";
    const char sKeyRCP[] = "Number of RCPs   =";
    const char sKeyCCP[] = "Number of CCPs   =";

    inp.seekg(0L);

    if (!ReadAfterPrefix(inp, sKeyTotal, NumberOfCriticals_))
        inp.seekg(0L);

    ReadAfterPrefix(inp, sKeyNACP, NumberOfNACP_);
    ReadAfterPrefix(inp, sKeyNNACP, NumberOfNNACP_);
    ReadAfterPrefix(inp, sKeyBCP, NumberOfBCP_);
    ReadAfterPrefix(inp, sKeyRCP, NumberOfRCP_);
    ReadAfterPrefix(inp, sKeyCCP, NumberOfCCP_);

    if (!NumberOfCriticals_)
        NumberOfCriticals_ = NumberOfNACP_ + NumberOfNNACP_ + NumberOfBCP_ + NumberOfRCP_ + NumberOfCCP_;

    this->ResetNumberOfAtoms(NumberOfCriticals_);

    return 1;
}

//
///////////////////////////////////////////////////////////////////////////////////////////////
// To be overriden to read information stored in the (file) stream
// int AcquireQTAIMFile::RequestData(vtkInformation *p_info
//    , vtkInformationVector **pp_ifv
//    , vtkInformationVector *p_ifv)
//
int AcquireQTAIMFile::ReadDataFrom(InputFile &inp, Molecule *pMol)
{
    // call base class:
    if (!this->Superclass::ReadDataFrom(inp, pMol))
        return 0; // pMol->Initialize()

    // Could it be a one-pass reading? Not al all...
    inp.seekg(0L);

    // Whatever else...
    return (this->ReadCriticalPoints(inp, pMol) == this->GetNumberOfAtoms()) ? 1 : 0;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////
//
int AcquireQTAIMFile::ReadCriticalPoints(InputFile &inp, Molecule *pMol)
{
    String one_line;
    vtkIdType nReadCrit = 0;
    if (!ScrollToPrefix(inp, "CP#", one_line))
    {
        vtkErrorMacro("AcquireQTAIMFile error: file " << this->GetFileName() << "does not contain critical structure data");
        return 0;
    }

    size_t idCP(0);
    do
    {
        String name;
        String AtomType;
        String str_type;
        String str_props;

        char equals;

        vtkIdType idElementAdd(0);
        double q0, q1, q2;
        CriticalPointType type;

        InputString inp_str(one_line);
        inp_str >> idCP       // == nCP + 1
            >> name           // "Coords"
            >> equals         // '='
            >> q0 >> q1 >> q2 // (xyz) of the point:
            ;
        // adjust the value of the CP index;
        if (!idCP || --idCP != nReadCrit)
            return 0;

        if (!GetLine(inp, str_type))
            break;

        // setup the critical molecular structure
        InputString inp_type(str_type);
        inp_type >> name // "Type"
            >> equals    // '='
            >> type      // (+3(or smth. else??),Sigma)
            >> name      // "((N|NN)A|B|R|C)CP"
            >> AtomType  //  "ElementIndex" ...
            ;
        size_t idx = name.find("ACP");
        if (idx != std::string::npos)
        {
            //    if(idx == 1) ... ["NACP"]
            //    if(idx == 2) ... ["NNACP"] ??? Never ever seen this beast
            if (type == cpTypeMaximum)
                idElementAdd = Elements::SymbolToNumber(AtomType.c_str());
        }
        else if (!name.find("BCP"))
        {
            // -> AtomType1 // exactly the only
            if (type == cpTypeSaddleB)
                idElementAdd = 2; // fictituous He
        }
        else if (!name.find("RCP"))
        {
            // -> AtomType1 AtomType2  ???  may be greater than 3 atoms
            if (type == cpTypeSaddleR)
                idElementAdd = 10; // fictituous Ne
        }
        else if (!name.find("CCP"))
        {
            // -> AtomType1 AtomType2 -> should be greater than 3 atoms
            if (type == cpTypeMininum)
                idElementAdd = 18; // fictituous Ar
        }
        else
            return 0;
        // enter here to input the critical data point-by-point
        if (idElementAdd)
            pMol->AppendAtom(idElementAdd, q0, q1, q2);
        ++nReadCrit;
        if (!GatherNonEmpty(inp, str_props))
            return 0;
        // below is the simplest case of skipping this info:
        // ScrollToEmpty(inp);
    } while (ScrollToPrefix(inp, "CP#", one_line));

    return idCP;
}