#ifndef __View_Structure_h__
#define __View_Structure_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QVTKOpenGLNativeWidget.h>

#include <vtkRenderWindow.h>
#include <vtkImageWriter.h>

#include <vtkRenderer.h>
#include <vtkColor.h>

#include <vtkActor.h>
#include <vtkLODActor.h>

#include "Molecule.h"
#include "MoleculeMapper.h"

#include <vtkNew.h>
#include <vtkSmartPointer.h>

using namespace vtk;

typedef vtkNew<MoleculeMapper> NewMoleculeMapper;
typedef vtkSmartPointer<MoleculeMapper> AMoleculeMapper;

typedef vtkNew<vtkRenderer> NewRenderer;
typedef vtkSmartPointer<vtkRenderer> ARenderer;

typedef vtkNew<vtkActor> NewActor;
typedef vtkSmartPointer<vtkActor> AnActor;

typedef vtkNew<vtkLODActor> NewLODActor;
typedef vtkSmartPointer<vtkLODActor> ALODActor;

class ViewMolecule : public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    ViewMolecule(QWidget * /*parent*/ = nullptr);

    bool initRendering(Molecule * /*pMol*/ = nullptr);
    bool resetMoleculeStyle(const StyleMapMolecule &style);

    bool exportImageTo(vtkImageWriter * /*pIW*/, bool /*bAlpha*/ = true);

    vtkColor3d getBgColor() const;
    vtkColor3d &BgColor();

    void ProjectParallel();
    void ProjectPerspective();

private:
    NewRenderer renderBg_;
    vtkColor3d colorBg_;

    StyleMapMolecule styleMol_;
    NewRenderer renderMol_;
    // NewLODActor actorMol_;
    NewActor actorMol_;
    AMoleculeMapper mapMol_;
};

#endif // !__View_Structure_h__