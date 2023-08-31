#ifndef Acquire_File_XYZ_h__
#define Acquire_File_XYZ_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FormatFile.h"
#include "AcquireFileBase.h"
#include <vtkXYZMolReader.h>
#include <vtkXYZMolReader2.h>

// using AcquireFileXYZ = vtkXYZMolReader2;

class AcquireFileXYZ;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<AcquireFileXYZ> NewAcquireXYZ;
typedef vtkSmartPointer<AcquireFileXYZ> ToAcquireXYZ;

#include <MakeBonds.h> // bottom of file...

/*=========================================================================
  Program:   Visualization Toolkit
  Module:    AcquireFileXYZ.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
/**
 * @class   AcquireFileXYZ
 * @brief   read Molecular Data files
 *
 * AcquireFileXYZ is a source object that reads Molecule files
 * The reader will detect multiple timesteps in an XYZ molecule file.
 *
 * @par Thanks:
 * Dr. Jean M. Favre who developed and contributed this class
 */
#include <istream> // for std::istream
#include <vector>  // for std::vector

// namespace vtk
// {

class /* VTKIOCHEMISTRY_EXPORT*/ AcquireFileXYZ
    : public AcquireFileBase
{
public:
  static AcquireFileXYZ *New();
  vtkTypeMacro(AcquireFileXYZ, AcquireFileBase);
  void PrintSelf(ostream &os, vtkIndent indent) override;

protected:
  explicit AcquireFileXYZ();
  ~AcquireFileXYZ() override = default;

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
  AcquireFileXYZ(const AcquireFileXYZ &) = delete;
  void operator=(const AcquireFileXYZ &) = delete;
};

// }; // namespace vtk

template <class Host>
bool ParseFileXYZTo(Path a_path, Host &host)
{
  NewAcquireXYZ read;
  read->resetPath(a_path.c_str());
  NewMolecule new_mol;
  read->SetOutput(new_mol);
  NewMakeBonds mkbonds;
  mkbonds->SetInputData(new_mol);
  mkbonds->SetOutput(host.getMolecule());
  read->Update();
  mkbonds->Update();
  return bool(host.getMolecule()->GetNumberOfAtoms() > 0);
}

#endif // !Acquire_File_XYZ_h__
