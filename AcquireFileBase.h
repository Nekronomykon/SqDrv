#ifndef AcquireFile_Base_h__
#define AcquireFile_Base_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "MoleculeSource.h"

#include "ImplPathName.h"

class AcquireFileBase
    : public MoleculeSource,
      public ImplPathName<AcquireFileBase>
{
public:
  static AcquireFileBase *New();
  vtkTypeMacro(AcquireFileBase, MoleculeSource);
  void PrintSelf(ostream &os, vtkIndent indent) override;
  //
protected:
  explicit AcquireFileBase(int /* nOuts */ = 1);
  ~AcquireFileBase() override = default;
};

#endif // !AcquireFile_Base_h__
