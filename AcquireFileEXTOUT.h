#ifndef Acquire_File_EXTOUT_h__
#define Acquire_File_EXTOUT_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

/*=========================================================================
  Program:   Visualization Toolkit
  Module:    AcquireFileEXTOUT.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
/**
 * @class   AcquireFileEXTOUT
 * @brief   read Molecular Data files
 *
 * AcquireFileEXTOUT is a source object that reads Molecule files
 * The reader will detect multiple timesteps in an EXTOUT molecule file.
 *
 * @par Thanks:
 * Dr. Jean M. Favre who developed and contributed this class
 */
#include "AcquireQTAIMFile.h"

#include <istream> // for std::istream
#include <vector>  // for std::vector

class vtkMolecule;

namespace vtk
{

    class /* VTKIOCHEMISTRY_EXPORT*/ AcquireFileEXTOUT
        : public AcquireQTAIMFile
    {
    public:
        static AcquireFileEXTOUT *New();
        vtkTypeMacro(AcquireFileEXTOUT, AcquireQTAIMFile);
        void PrintSelf(ostream &os, vtkIndent indent) override;

    protected:
        explicit AcquireFileEXTOUT();
        ~AcquireFileEXTOUT() override = default;

        int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
        int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;

    private:
        enum
        {
            NumLinesHeader = 23
        };
        AcquireFileEXTOUT(const AcquireFileEXTOUT &) = delete;
        void operator=(const AcquireFileEXTOUT &) = delete;
    };

}; // namespace vtk

#endif // !Acquire_File_EXTOUT_h__
