#ifndef Acquire_QTAIMFile_h__
#define Acquire_QTAIMFile_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkObjectFactory.h>
#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include <vtkMoleculeAlgorithm.h>
#include <vtkStringArray.h>

#include <vtksys/FStream.hxx>

#include "AcquireFile.h"

namespace vtk
{
    class AcquireQTAIMFile
    : public AcquireFile
    {
        protected:
        AcquireQTAIMFile()
    };
}; // namespace vtk

#endif // !Acquire_QTAIMFile_h__