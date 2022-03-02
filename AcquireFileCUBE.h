#ifndef Acquire_File_CUBE_h__
#define Acquire_File_CUBE_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

/*=========================================================================
  Program:   Visualization Toolkit
  Module:    AcquireFileCUBE.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
/**
 * @class   AcquireFileCUBE
 * @brief   Read a Gaussian Cube file and output
 * 
 * vtkMolecule object and a vtkImageData
 *
 *
 * @par Thanks:
 * Dr. Jean M. Favre who developed and contributed this class. */

#include "AcquireMoleculeFile.h"
// #include "AcquireQTAIMFile.h" --> perspective...

class vtkImageData;

namespace vtk
{
class Molecule;

    class /*VTKIOCHEMISTRY_EXPORT*/ AcquireFileCUBE
        : public AcquireMoleculeFile
    {
    public:
        static AcquireFileCUBE *New();
        vtkTypeMacro(AcquireFileCUBE, AcquireMoleculeFile);
        void PrintSelf(ostream &os, vtkIndent indent) override;

        ///@{

    /**
    * Get/Set the output (vtkImageData) that the reader will fill
    */
        vtkImageData *GetGridOutput();

        ///@{

    protected:
        explicit AcquireFileCUBE();
        ~AcquireFileCUBE() override = default;

        int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
        int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
        int FillOutputPortInformation(int, vtkInformation *) override;

    private:
        AcquireFileCUBE(const AcquireFileCUBE &) = delete;
        void operator=(const AcquireFileCUBE &) = delete;
    };

}; // namespace vtk

#endif // !Acquire_File_CUBE_h__
