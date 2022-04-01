#ifndef MapMoleculeOpenGL_h__
#define MapMoleculeOpenGL_h__

/*=========================================================================

  Program:   Visualization Toolkit

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   MapMoleculeOpenGL
 * @brief   An accelerated class for rendering molecules
 *
 * -> ScrewDriver te Blackheadborough:
 * A MapMolecule that uses imposters to do the rendering. It uses
 * vtkOpenGLSphereMapper and vtkOpenGLStickMapper to do the rendering.
 * Stick representations contain unexpected circular holes. Looking for 
 * the solution...
 */

// #include "vtkDomainsChemistryOpenGL2Module.h" // For export macro
#include "MapMolecule.h"

#include "vtkNew.h" // For vtkNew

class vtkOpenGLSphereMapper;
class vtkOpenGLStickMapper;

namespace vtk
{

    class /*VTKDOMAINSCHEMISTRYOPENGL2_EXPORT*/ MapMoleculeOpenGL : public MapMolecule
    {
    public:
        static MapMoleculeOpenGL *New();
        vtkTypeMacro(MapMoleculeOpenGL, MapMolecule);
        void PrintSelf(ostream &os, vtkIndent indent) override;

        ///@{
        /**
         * Reimplemented from base class
         */
        void Render(vtkRenderer *, vtkActor *) override;
        void ReleaseGraphicsResources(vtkWindow *) override;
        ///@}

        /**
         * provide access to the underlying mappers
         */
        vtkOpenGLSphereMapper *GetFastAtomMapper() { return this->FastAtomMapper; }
        /**
         * allows a mapper to update a selections color buffers
         * Called from a prop which in turn is called from the selector
         */
        void ProcessSelectorPixelBuffers(
            vtkHardwareSelector *sel, std::vector<unsigned int> &pixeloffsets, vtkProp *prop) override;

        /**
         * Helper method to set ScalarMode on both FastAtomMapper and FastBondMapper.
         * true means VTK_COLOR_MODE_MAP_SCALARS, false VTK_COLOR_MODE_DIRECT_SCALARS.
         */
        void SetMapScalars(bool map) override;

    protected:
        MapMoleculeOpenGL();
        ~MapMoleculeOpenGL() override;

        void UpdateAtomGlyphPolyData() override;
        void UpdateBondGlyphPolyData() override;

        ///@{
        /**
         * Internal mappers
         */
        vtkNew<vtkOpenGLSphereMapper> FastAtomMapper;
        vtkNew<vtkOpenGLStickMapper> FastBondMapper;
        ///@}

    private:
        MapMoleculeOpenGL(const MapMoleculeOpenGL &) = delete;
        void operator=(const MapMoleculeOpenGL &) = delete;

    }; // MapMoleculeOpenGL

}; // namespace vtk

#endif // !MapMoleculeOpenGL_h__
