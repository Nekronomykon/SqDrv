#ifndef AcquireFile_Base_h__
#define AcquireFile_Base_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkStringArray.h>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include "SourceOfMolecule.h"

#include "ImplPathName.h"

class AcquireFileBase
    : public SourceOfMolecule,
      public ImplPathName<AcquireFileBase>
{
public:
  static AcquireFileBase *New();
  vtkTypeMacro(AcquireFileBase, SourceOfMolecule);
  void PrintSelf(ostream &os, vtkIndent indent) override;
  //
  vtkIdType GetNumberOfAtoms() const;
  vtkIdType resetNumberOfAtoms(vtkIdType /*id*/ = 0L);
  //
  String getTitle()const;
  String resetTitle(String /*name*/ = String());
  String& Title();

protected:
  explicit AcquireFileBase(int /* nOuts */ = 1);
  ~AcquireFileBase() override = default;
  //
  vtkIdType &NumberOfAtoms();

  // ----------------------------------------------------------------------------------------------------
  // To be overriden to read information stored in the (file) stream
  // ----------------------------------------------------------------------------------------------------
  int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  virtual int ReadSizesFrom(InputFile & /*inp*/);
  // ----------------------------------------------------------------------------------------------------
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  virtual int ReadDataFrom(InputFile & /*inp*/, Molecule * /*ptrMol*/);
  virtual int OnReadDataComplete(Molecule * /* ptrMol */);
  // ----------------------------------------------------------------------------------------------------

private:
  String title_ = String();
  vtkIdType numAtoms_ = -1;
  vtkNew<vtkStringArray> nameAtoms_;

private:
  AcquireFileBase(const AcquireFileBase &) = delete;
  void operator=(const AcquireFileBase &) = delete;
};

#endif // !AcquireFile_Base_h__
