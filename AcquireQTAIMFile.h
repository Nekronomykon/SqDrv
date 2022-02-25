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
#include "CriticalPoint.h"

namespace vtk
{

    class AcquireQTAIMFile
        : public AcquireFile
    {
    protected:
        explicit AcquireQTAIMFile();

    public:
        static AcquireQTAIMFile *New();
        vtkTypeMacro(AcquireQTAIMFile, AcquireFile);
        void PrintSelf(ostream &os, vtkIndent indent) override;

        vtkIdType GetNumberOfACP(void) const { return NumberOfNACP_ + NumberOfNNACP_; }
        vtkIdType GetNumberOfNACP(void) const { return NumberOfNACP_; }
        vtkIdType GetNumberOfNNACP(void) const { return NumberOfNNACP_; }
        vtkIdType GetNumberOfBCP(void) const { return NumberOfBCP_; }
        vtkIdType GetNumberOfRCP(void) const { return NumberOfRCP_; }
        vtkIdType GetNumberOfCCP(void) const { return NumberOfCCP_; }

    protected:
        // vtkIdType &NumberOfAtoms(void) { return NumberOfAtoms_; }

    private:
        vtkIdType NumberOfNACP_ = 0;
        vtkIdType NumberOfNNACP_ = 0;
        vtkIdType NumberOfBCP_ = 0;
        vtkIdType NumberOfRCP_ = 0;
        vtkIdType NumberOfCCP_ = 0;
    };
}; // namespace vtk

#endif // !Acquire_QTAIMFile_h__