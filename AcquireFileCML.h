#ifndef AcquireFile_CML_h__
#define AcquireFile_CML_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FormatFile.h"
#include "AcquireFileBase.h"
#include <vtkCMLMoleculeReader.h>

using AcquireFileCML = vtkCMLMoleculeReader;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<AcquireFileCML> NewAcquireCML;
typedef vtkSmartPointer<AcquireFileCML> ToAcquireCML;

template <class Host>
bool ParseFileCMLTo(Path a_path, Host &host)
{
  NewAcquireCML read;
  read->SetFileName(a_path.c_str());
  read->SetOutput(host.getMolecule());
  read->Update();
  host.resetTitle(/* read->getTitle() */);
  return bool(host.getMolecule()->GetNumberOfAtoms() > 0);
}

#endif // !AcquireFile_CML_h__
