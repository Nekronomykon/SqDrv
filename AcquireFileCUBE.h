#ifndef Acquire_File_CUBE_h__
#define Acquire_File_CUBE_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FormatFile.h"

#include <vtkGaussianCubeReader.h>
#include <vtkGaussianCubeReader2.h>

using AcquireFileCUBE = vtkGaussianCubeReader2;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<AcquireFileCUBE>           NewAcquireCUBE;
typedef vtkSmartPointer<AcquireFileCUBE>   ToAcquireCUBE;

template <class Host>
bool ParseFileCUBETo(Path a_path, Host &host)
{
    NewAcquireCUBE read;
    read->SetFileName(a_path.c_str());
    read->SetOutput(host.getMolecule());
    read->Update();
  return bool(host.getMolecule()->GetNumberOfAtoms() > 0);
}

#endif // !Acquire_File_CUBE_h__
