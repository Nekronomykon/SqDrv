#ifndef Acquire_QTAIM_File_h__
#define Acquire_QTAIM_File_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "AcquireFileBase.h"

#include <vtkObjectFactory.h>
#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include "CriticalPoint.h"

#include <vtkStringArray.h>
#include <vtksys/FStream.hxx>

class vtkInformation;
class vtkInformationVector;

// namespace vtk
//{

class AcquireFileAIM
    : public AcquireFileBase
{
protected:
  explicit AcquireFileAIM();

public:
  static AcquireFileAIM *New();
  vtkTypeMacro(AcquireFileAIM, AcquireFileBase);
  void PrintSelf(ostream &os, vtkIndent indent) override;

  vtkIdType GetNumberOfCP(void) const { return NumberOfCriticals_; }

  vtkIdType GetNumberOfACP(void) const { return NumberOfNACP_ + NumberOfNNACP_; }
  // --
  vtkIdType GetNumberOfNACP(void) const { return NumberOfNACP_; }
  vtkIdType GetNumberOfNNACP(void) const { return NumberOfNNACP_; } // generally 0
  // --
  vtkIdType GetNumberOfBCP(void) const { return NumberOfBCP_; }
  vtkIdType GetNumberOfRCP(void) const { return NumberOfRCP_; }
  vtkIdType GetNumberOfCCP(void) const { return NumberOfCCP_; }
  // ===========================================================================

  int ReadCriticalPoints(InputFile & /*inp*/, Molecule * /*ptrMol*/);

protected:
  // ----------------------------------------------------------------------------------------------------
  // To be overriden to read information stored in the (file) stream
  // ----------------------------------------------------------------------------------------------------
  // int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  int ReadSizesFrom(InputFile & /*inp*/) override;
  // ----------------------------------------------------------------------------------------------------
  // int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  int ReadDataFrom(InputFile & /*inp*/, Molecule * /*ptrMol*/) override;
  // ----------------------------------------------------------------------------------------------------
  // virtual int ReadQTAIMSizes(InputFile& /* inp */);

  // private:
  vtkIdType NumberOfCriticals_ = 0;
  vtkIdType NumberOfNACP_ = 0;
  vtkIdType NumberOfNNACP_ = 0;
  vtkIdType NumberOfBCP_ = 0;
  vtkIdType NumberOfRCP_ = 0;
  vtkIdType NumberOfCCP_ = 0;
};
// }; // namespace vtk

#endif // !Acquire_QTAIM_File_h__
