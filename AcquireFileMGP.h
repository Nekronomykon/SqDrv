#ifndef Acquire_File_MGP_h__
#define Acquire_File_MGP_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

/*=========================================================================
  Program:   Visualization Toolkit
  Module:    AcquireFileMGP.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
/**
 * @class   AcquireFileMGP
 * @brief   read Molecular Data files
 *
 * AcquireFileMGP is a source object that reads Molecule files
 * The reader will detect multiple timesteps in an MGP molecule file.
 *
 * @par Thanks:
 * Dr. Jean M. Favre who developed and contributed this class
 */
#include "AcquireQTAIMFile.h"

#include <istream> // for std::istream
#include <vector>  // for std::vector

namespace vtk
{

    class /* VTKIOCHEMISTRY_EXPORT*/ AcquireFileMGP
        : public AcquireQTAIMFile
    {
    public:
        static AcquireFileMGP *New();
        vtkTypeMacro(AcquireFileMGP, AcquireQTAIMFile);
        void PrintSelf(ostream &os, vtkIndent indent) override;

    protected:
        explicit AcquireFileMGP();
        ~AcquireFileMGP() override = default;

    private:
        AcquireFileMGP(const AcquireFileMGP &) = delete;
        void operator=(const AcquireFileMGP &) = delete;
    };

}; // namespace vtk

#endif // !Acquire_File_MGP_h__
