#ifndef AcquireFile_HIN_h__
#define AcquireFile_HIN_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FormatFile.h"
#include "AcquireFileBase.h"
// #include <vtkHINMolReader.h>
// #include <vtkHINMolReader2.h>

// using AcquireFileHIN = vtkHINMolReader2;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include <MakeBonds.h> // bottom of file...

/*=========================================================================
  Program:   Visualization Toolkit
  Module:    AcquireFileHIN.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
/**
 * @class   AcquireFileHIN
 * @brief   read Molecular Data files
 *
 * AcquireFileHIN is a source object that reads Molecule files
 * The reader will detect multiple timesteps in an HIN molecule file.
 *
 * @par Thanks:
 * Dr. Jean M. Favre who developed and contributed this class
 */
#include <istream> // for std::istream
#include <vector>  // for std::vector

// namespace vtk
// {

class /* VTKIOCHEMISTRY_EXPORT*/ AcquireFileHIN
    : public AcquireFileBase
{
public:
  static AcquireFileHIN *New();
  vtkTypeMacro(AcquireFileHIN, AcquireFileBase);
  void PrintSelf(ostream &os, vtkIndent indent) override;

protected:
  explicit AcquireFileHIN();
  ~AcquireFileHIN() override = default;

  // ----------------------------------------------------------------------------------------------------
  // To be overriden to read information stored in the (file) stream
  // ----------------------------------------------------------------------------------------------------
  // int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  int ReadSizesFrom(InputFile & /*inp*/) override;
  // ----------------------------------------------------------------------------------------------------
  // int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  int ReadDataFrom(InputFile & /*inp*/, Molecule * /* pMol */) override;
  // int OnReadDataComplete(Molecule * /* ptrMol */) override;
  // ----------------------------------------------------------------------------------------------------

private:
  AcquireFileHIN(const AcquireFileHIN &) = delete;
  void operator=(const AcquireFileHIN &) = delete;
};

// }; // namespace vtk

typedef vtkNew<AcquireFileHIN> NewAcquireHIN;
typedef vtkSmartPointer<AcquireFileHIN> ToAcquireHIN;

template <class Host>
bool ParseFileHINTo(Path a_path, Host &host)
{
  NewAcquireHIN read;
  read->resetPath(a_path);
  read->SetOutput(host.getMolecule());
  read->Update();
  host.resetTitle(read->getTitle());
  return bool(host.getMolecule()->GetNumberOfAtoms() > 0);
}

#endif // !AcquireFile_HIN_h__
