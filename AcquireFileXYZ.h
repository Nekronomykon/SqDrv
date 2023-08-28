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

using AcquireFileXYZ = vtkXYZMolReader2;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<AcquireFileXYZ> NewAcquireXYZ;
typedef vtkSmartPointer<AcquireFileXYZ> ToAcquireXYZ;

#include <MakeBonds.h>

template <class Host>
bool ParseFileXYZTo(Path a_path, Host &host)
{
  NewAcquireXYZ read;
  read->SetFileName(a_path.c_str());
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
