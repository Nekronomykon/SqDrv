#include "AcquireQTAIMFile.h"

#include "Elements.h"
#include "Molecule.h"

#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkAtom.h>
#include <vtkIdTypeArray.h>

#include <vtkSetGet.h>

#include <vector>

using namespace vtk;
using namespace std;

//------------------------------------------------------------------------------
vtkStandardNewMacro(AcquireQTAIMFile);

//------------------------------------------------------------------------------
AcquireQTAIMFile::AcquireQTAIMFile() : AcquireFileBase(1) {}
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

  this->resetNumberOfAtoms(NumberOfCriticals_);

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
  // pMol->resetUnits(Molecule::Bohr);

  // Whatever else...
  return (this->ReadCriticalPoints(inp, pMol)) ? 1 : 0;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////
//
int AcquireQTAIMFile::ReadCriticalPoints(InputFile &inp, Molecule *pMol)
{
  String one_line;
  vtkIdType nReadCP = 0;
  if (!ScrollToPrefix(inp, "CP#", one_line))
  {
    vtkErrorMacro("AcquireQTAIMFile error: file " << this->getPath().string() << "does not contain critical structure data");
    return 0;
  }

  size_t idCP(0);
  String AtomType;
  char equals;
  // double q0, q1, q2;
  double xyz[3];
  do
  {
    String skip;
    String str_props;

    vtkIdType idElementAdd(0);
    CriticalPointType type;

    InputString inp_point(one_line);
    inp_point >> idCP                 // == nCP + 1
        >> skip                       // "Coords"
        >> equals                     // '='
        >> xyz[0] >> xyz[1] >> xyz[2] // (xyz) of the point:
        ;
    // adjust the value of the CP index;
    // if (!idCP || --idCP != nReadCP)
    // return 0;

    vtkAtom atom_new = pMol->AppendAtom(0, xyz); // Hush-sh... too fast

    String str_type;
    if (!GetLine(inp, str_type))
    {
      vtkErrorMacro("AcquireQTAIMFile error: CP record #"
                    << ++nReadCP
                    << "in file " << this->getPath().string()
                    << "does not contain the type data");
      return 0;
    }
    InputString inp_type(str_type);
    // setup the critical molecular structure
    inp_type >> skip // "Type"
        >> equals    // '='
        >> type      // (+3(or smth. else??),Sigma)
        >> skip      // "((N|NN)A|B|R|C)CP"
        >> AtomType  //  "ElementIndex" ...
        ;
    size_t idx = skip.find("ACP");
    char *str_aux(nullptr);
    vtkIdType idAtomAux;
    if (idx != String::npos)
    {
      //    if(idx == 1) ... ["NACP"]
      //    if(idx == 2) ... ["NNACP"] ??? Never ever seen this beast
      if (type == cpTypeMaximum)
        idElementAdd = Elements::SymbolToNumber(AtomType.c_str(), &str_aux);
      else
      {
        vtkErrorMacro("AcquireQTAIMFile error: CP record #"
                      << ++nReadCP
                      << "in file " << this->getPath().string()
                      << "has NameType=" << skip
                      << " incompatible with Type=(" << type.real() << "," << type.imag() << ")");
        return 0;
      }

      // idAtomAux = strtol(str_aux, &str_aux, 10);
    }
    else if (!skip.find("BCP"))
    {
      // -> AtomType1 // exactly the only
      if (type == cpTypeSaddleB)
        idElementAdd = 0;
      // idElementAdd = 18; // ??? fictituous Ar
      // pMol->AppendBond(idCP)
      else
      {
        vtkErrorMacro("AcquireQTAIMFile error: CP record #"
                      << ++nReadCP
                      << "in file " << this->getPath().string()
                      << "has NameType=" << skip
                      << " incompatible with Type=(" << type.real() << "," << type.imag() << ")");
        return 0;
      }
      vtkIdType idE0 = Elements::SymbolToNumber(AtomType.c_str(), &str_aux);
      vtkIdType idA0 = strtol(str_aux, &str_aux, 10);
      --idA0;               // number to index
      inp_type >> AtomType; // next atom; only two-center bonds are included here
      vtkIdType idE1 = Elements::SymbolToNumber(AtomType.c_str(), &str_aux);
      vtkIdType idA1 = strtol(str_aux, &str_aux, 10);
      --idA1; // number to index
      // adding a simplex
      pMol->AppendBond(atom_new.GetId(), idA0, 0); // TODO: Try to rely upon the AppendBondCP(...) call below;
      pMol->AppendBond(atom_new.GetId(), idA1, 0); // special bond type CriticalPoint::BondCriticalPoint required
      // pMol->AppendBondCP(idA0, idA1, xyz);
    }
    else if (!skip.find("RCP"))
    {
      // -> AtomType1 AtomType2  ???  may be greater than 3 atoms
      if (type == cpTypeSaddleR)
        idElementAdd = Elements::idNone; // fictituous point
      else
      {
        vtkErrorMacro("AcquireQTAIMFile error: CP record #" << ++nReadCP
                                                            << "in file " << this->getPath().string()
                                                            << "has NameType=" << skip
                                                            << " incompatible with Type=(" << type.real() << "," << type.imag() << ")");
        return 0;
      }

      vector<vtkIdType> atomsRing;
      atomsRing.reserve(3);

      do
      {
        vtkIdType idE0 = Elements::SymbolToNumber(AtomType.c_str(), &str_aux);
        vtkIdType idA0 = strtol(str_aux, &str_aux, 10);
        --idA0;
        atomsRing.push_back(idA0);
        pMol->AppendBond(atom_new.GetId(), idA0, 1);
      } while (inp_type >> AtomType);
    }
    else if (!skip.find("CCP"))
    {
      // -> AtomType1 AtomType2 -> should be greater than 3 atoms
      if (type == cpTypeMinimum)
        idElementAdd = 2; // fictituous He; local minimum
      else
      {
        vtkErrorMacro("AcquireQTAIMFile error: CP record #" << ++nReadCP
                                                            << "in file " << this->getPath().string()
                                                            << "has incompatible NameType=" << skip
                                                            << " and Type=(" << type.real() << "," << type.imag() << ")");
        return 0;
      }

      vector<vtkIdType> atomsCage;
      atomsCage.reserve(3);

      do
      {
        vtkIdType idE0 = Elements::SymbolToNumber(AtomType.c_str(), &str_aux);
        vtkIdType idA0 = strtol(str_aux, &str_aux, 10);
        --idA0;
        atomsCage.push_back(idA0);
        pMol->AppendBond(atom_new.GetId(), idA0, 1);
      } while (inp_type >> AtomType);
    }
    else
    {
      vtkErrorMacro("AcquireQTAIMFile error: CP record #" << ++nReadCP
                                                          << "in file " << this->getPath().string()
                                                          << "has incompatible NameType=" << skip
                                                          << " and Type=(" << type.real() << "," << type.imag() << ")");
      return 0;
    } /*
     // enter here to input the critical data point-by-point
     if (!idElementAdd)
         return 0;
     pMol->AppendAtom(idElementAdd, q0, q1, q2);
     if (!GatherNonEmpty(inp, str_props))
         return 0;
     // below is the simplest case of skipping this info:
     // ScrollToEmpty(inp); */
    if (idElementAdd)
      atom_new.SetAtomicNumber(idElementAdd);
    // atom_new.MakeAuxiliaryPoint(idPointType);
    ++nReadCP;
  } while (ScrollToPrefix(inp, "CP#", one_line));

  return nReadCP;
}