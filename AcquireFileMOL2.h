#ifndef AcquireFile_MOL2_h__
#define AcquireFile_MOL2_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FormatFile.h"
#include "AcquireFileBase.h"

#include <vtkNew.h>
#include <vtkSmartPointer.h>

class AcquireFileMOL2
: public AcquireFileBase
{
public:
  static AcquireFileMOL2 *New();
  vtkTypeMacro(AcquireFileMOL2, AcquireFileBase);
  void PrintSelf(ostream &os, vtkIndent indent) override;

protected:
  explicit AcquireFileMOL2();
  ~AcquireFileMOL2() override = default;

  // ----------------------------------------------------------------------------------------------------
  // To be overriden to read information stored in the (file) stream
  // ----------------------------------------------------------------------------------------------------
  // int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  // int ReadSizesFrom(InputFile & /*inp*/) override;
  // ----------------------------------------------------------------------------------------------------
  // int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  // int ReadDataFrom(InputFile & /*inp*/, Molecule * /* pMol */) override;
  // int OnReadDataComplete(Molecule * /* ptrMol */) override;
  // ----------------------------------------------------------------------------------------------------

private:
  AcquireFileMOL2(const AcquireFileMOL2 &) = delete;
  void operator=(const AcquireFileMOL2 &) = delete;

};

typedef vtkNew<AcquireFileMOL2> NewAcquireMOL2;
typedef vtkSmartPointer<AcquireFileMOL2> ToAcquireMOL2;

template <class Host>
bool ParseFileMOL2To(Path a_path, Host &host)
{
  NewAcquireMOL2 read;
  read->resetPath(a_path);
  read->SetOutput(host.getMolecule());
  read->Update();
  host.resetTitle(read->getTitle());
  return bool(host.getMolecule()->GetNumberOfAtoms() > 0);
}

#endif // !AcquireFile_MOL2_h__

