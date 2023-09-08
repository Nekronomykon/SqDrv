#ifndef AcquireFile_WFX_h__
#define AcquireFile_WFX_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

/*=========================================================================
  Program:   Visualization Toolkit
  Module:    AcquireFileWFX.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
/**
 * @class   AcquireFileWFX
 * @brief   read Molecular Data files
 *
 * AcquireFileWFX is a source object that reads Molecule files
 * The reader will detect multiple timesteps in an WFN molecule file.
 *
 * @par Thanks:
 * Dr. Jean M. Favre who developed and contributed this class
 */
#include "AcquireFileBase.h"

class AcquireFileWFX;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<AcquireFileWFX> NewAcquireWFX;
typedef vtkSmartPointer<AcquireFileWFX> ToAcquireWFX;

#include <istream> // for std::istream
#include <vector>  // for std::vector

class /* VTKIOCHEMISTRY_EXPORT*/ AcquireFileWFX
    : public AcquireFileBase
{
public:
  static AcquireFileWFX *New();
  vtkTypeMacro(AcquireFileWFX, AcquireFileBase);
  void PrintSelf(ostream &os, vtkIndent indent) override;

protected:
  explicit AcquireFileWFX();
  ~AcquireFileWFX() override = default;

  // ----------------------------------------------------------------------------------------------------
  // To be overriden to read information stored in the (file) stream
  // ----------------------------------------------------------------------------------------------------
  // int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  int ReadSizesFrom(InputFile & /*inp*/) override;
  // ----------------------------------------------------------------------------------------------------
  // int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  int ReadDataFrom(InputFile & /*inp*/, Molecule * /* pMol */) override;
  // int OnReadDataComplete(Molecule* /* ptrMol */) override;
  // ----------------------------------------------------------------------------------------------------

private:
  AcquireFileWFX(const AcquireFileWFX &) = delete;
  void operator=(const AcquireFileWFX &) = delete;
};

template <class Host>
bool ParseFileWFXTo(Path a_path, Host &host)
{
  NewAcquireWFX read;
  read->resetPath(a_path);
  read->SetOutput(host.getMolecule());
  read->Update();
  host.resetTitle(read->getTitle());
  return bool(host.getMolecule()->GetNumberOfAtoms() > 0);
}

#endif // !AcquireFile_WFX_h__
