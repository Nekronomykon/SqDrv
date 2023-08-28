#ifndef ExportFile_Base_h__
#define ExportFile_Base_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "MoleculeSource.h"

#include "ImplPathName.h"

class ExportFileBase
    : public MoleculeSource,
      public ImplPathName<ExportFileBase>
{
public:
  static ExportFileBase *New();
  vtkTypeMacro(ExportFileBase, MoleculeSource);
  void PrintSelf(ostream &os, vtkIndent indent) override;
  //
protected:
  explicit ExportFileBase(int /* nOuts */ = 1);
  ~ExportFileBase() override = default;
};

#endif // !ExportFile_Base_h__
