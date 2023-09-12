#ifndef AcquireFile_SUM_h__
#define AcquireFile_SUM_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

/*=========================================================================
  Program:   Visualization Toolkit
  Module:    AcquireFileSUM.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
/**
 * @class   AcquireFileSUM
 * @brief   read Molecular Data files
 *
 * AcquireFileSUM is a source object that reads Molecule files
 * The reader will detect multiple timesteps in an SUM molecule file.
 *
 * @par Thanks:
 * Dr. Jean M. Favre who developed and contributed this class
 */
#include "AcquireFileAIM.h"

#include <istream> // for std::istream
#include <vector>  // for std::vector

#include <vtkNew.h>
#include <vtkSmartPointer.h>

// namespace vtk
// {

class /* VTKIOCHEMISTRY_EXPORT*/ AcquireFileSUM
    : public AcquireFileAIM
{
public:
  static AcquireFileSUM *New();
  vtkTypeMacro(AcquireFileSUM, AcquireFileAIM);
  void PrintSelf(ostream &os, vtkIndent indent) override;

protected:
  explicit AcquireFileSUM();
  ~AcquireFileSUM() override = default;

  // ----------------------------------------------------------------------------------------------------
  // To be overriden to read information stored in the (file) stream
  // ----------------------------------------------------------------------------------------------------
  // int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) /*override*/;
  // int ReadSizesFrom(InputFile & /*inp*/) override;
  // ----------------------------------------------------------------------------------------------------
  // int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) /*override*/;
  // int ReadDataFrom(InputFile & /*inp*/, Molecule * /*ptrMol*/) override;
  // int OnReadDataComplete(Molecule* /* ptrMol */) override;
  // ----------------------------------------------------------------------------------------------------

private:
  enum
  {
    NumLinesHeader = 33
  };
  AcquireFileSUM(const AcquireFileSUM &) = delete;
  void operator=(const AcquireFileSUM &) = delete;
};

typedef vtkNew<AcquireFileSUM> NewAcquireSUM;
typedef vtkSmartPointer<AcquireFileSUM> ToAcquireSUM;

template <class Host>
bool ParseFileSUMTo(Path a_path, Host &host)
{
  NewAcquireSUM read;
  read->resetPath(a_path);
  read->SetOutput(host.getMolecule());
  read->Update();
  host.resetTitle(read->getTitle());
  return bool(host.getMolecule()->GetNumberOfAtoms() > 0);
}

// }; // namespace vtk

#endif // !AcquireFile_SUM_h__
