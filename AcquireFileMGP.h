#ifndef AcquireFile_MGP_h__
#define AcquireFile_MGP_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

/*=========================================================================
  Program:   Visualization Toolkit
  Module:    AcquireFileMGP.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
/**
 * @class   AcquireFileMGP
 * @brief   read Molecular Data files
 *
 * AcquireFileMGP is a source object that reads Molecule files
 * The reader will detect multiple timesteps in an MGP molecule file.
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

class /* VTKIOCHEMISTRY_EXPORT*/ AcquireFileMGP
    : public AcquireFileAIM
{
public:
  static AcquireFileMGP *New();
  vtkTypeMacro(AcquireFileMGP, AcquireFileAIM);
  void PrintSelf(ostream &os, vtkIndent indent) override;

protected:
  explicit AcquireFileMGP();
  ~AcquireFileMGP() override = default;

  // ----------------------------------------------------------------------------------------------------
  // To be overriden to read information stored in the (file) stream
  // ----------------------------------------------------------------------------------------------------
  // int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  // int ReadSizesFrom(InputFile & /*inp*/) override;
  // ----------------------------------------------------------------------------------------------------
  // int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  // int ReadDataFrom(InputFile & /*inp*/, Molecule * /*ptrMol*/) override;
  // int OnReadDataComplete(Molecule* /* ptrMol */) override;
  // ----------------------------------------------------------------------------------------------------

private:
  AcquireFileMGP(const AcquireFileMGP &) = delete;
  void operator=(const AcquireFileMGP &) = delete;
};

typedef vtkNew<AcquireFileMGP> NewAcquireMGP;
typedef vtkSmartPointer<AcquireFileMGP> ToAcquireMGP;

template <class Host>
bool ParseFileMGPTo(Path a_path, Host &host)
{
  NewAcquireMGP read;
  read->resetPath(a_path);
  read->SetOutput(host.getMolecule());
  read->Update();
  host.resetTitle(read->getTitle());
  return bool(host.getMolecule()->GetNumberOfAtoms() > 0);
}

// }; // namespace vtk

#endif // !AcquireFile_MGP_h__
