#ifndef Acquire_File_XYZ_h__
#define Acquire_File_XYZ_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

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
#include "AcquireMoleculeFile.h"

#include <istream> // for std::istream
#include <vector>  // for std::vector

namespace vtk
{

    class /* VTKIOCHEMISTRY_EXPORT*/ AcquireFileXYZ 
    : public AcquireMoleculeFile
    {
    public:
        static AcquireFileXYZ *New();
        vtkTypeMacro(AcquireFileXYZ, AcquireMoleculeFile);
        void PrintSelf(ostream &os, vtkIndent indent) override;

    protected:
        explicit AcquireFileXYZ();
        ~AcquireFileXYZ() override = default;

        // ----------------------------------------------------------------------------------------------------
        // To be overriden to read information stored in the (file) stream
        // ----------------------------------------------------------------------------------------------------
        // int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
        int ReadSizesFrom(InputFile& /*inp*/) override;
        // ----------------------------------------------------------------------------------------------------
        // int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
        int ReadDataFrom(InputFile& /*inp*/, Molecule* /* pMol */) override;
        int OnReadDataComplete(Molecule* /* ptrMol */) override;
        // ----------------------------------------------------------------------------------------------------

    private:
        AcquireFileXYZ(const AcquireFileXYZ &) = delete;
        void operator=(const AcquireFileXYZ &) = delete;
    };

}; // namespace vtk

#endif // !Acquire_File_XYZ_h__
