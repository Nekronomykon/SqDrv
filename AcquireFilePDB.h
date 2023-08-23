#ifndef Acquire_File_PDB_h__
#define Acquire_File_PDB_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FormatFile.h"

#include <vtkPDBReader.h>

using AcquireFilePDB = vtkPDBReader;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<AcquireFilePDB>           NewAcquirePDB;
typedef vtkSmartPointer<AcquireFilePDB>   ToAcquirePDB;

template <class Host>
bool ParseFilePDBTo(Path a_path, Host &host)
{
    NewAcquirePDB read;
    read->SetFileName(a_path.c_str());
    read->SetOutput(host.getMolecule());
    read->Update();
  return bool(host.getMolecule()->GetNumberOfAtoms() > 0);
}

#endif // !Acquire_File_PDB_h__
