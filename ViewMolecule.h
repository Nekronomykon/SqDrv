#ifndef __View_Structure_h__
#define __View_Structure_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QVTKOpenGLNativeWidget.h>

#include <vtkRenderWindow.h>
#include <vtkImageWriter.h>

#include <vtkLabeledDataMapper.h>

#include <vtkRenderer.h>
#include <vtkColor.h>

#include <vtkActor.h>
#include <vtkLODActor.h>
#include <vtkActor2D.h>

#include "Molecule.h"
#include "MapMolecule.h"

#include <vtkStringArray.h>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<vtkRenderer> ANewRenderer;
typedef vtkSmartPointer<vtkRenderer> ARenderer;

typedef vtkNew<vtkActor> ANewActor;
typedef vtkSmartPointer<vtkActor> AnActor;

typedef vtkNew<vtkLODActor> ANewLODActor;
typedef vtkSmartPointer<vtkLODActor> ALODActor;

typedef vtkNew<vtkActor2D> ANewActor2D;
typedef vtkSmartPointer<vtkActor2D> AnActor2D;

typedef vtkNew<vtkLabeledDataMapper> ANewLabeledDataMapper;
typedef vtkSmartPointer<vtkLabeledDataMapper> ALabeledDataMapper;

typedef vtkNew<vtkStringArray> ANewStringArray;
typedef vtkSmartPointer<vtkStringArray> AStringArray;

using namespace vtk;

typedef vtkNew<MapMolecule> ANewMapOfMolecule;
typedef vtkSmartPointer<MapMolecule> AMapOfMolecule;

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
    ANewRenderer renderBg_;
    vtkColor3d colorBg_;

    ANewStringArray labels_;
    ANewLabeledDataMapper mapLabels_;
    ANewActor2D actorLabels_;

    StyleMapMolecule styleMol_;
    ANewRenderer renderMol_;
    // ANewLODActor actorMol_;
    ANewActor actorMol_;
    AMapOfMolecule mapMol_;
};

#endif // !__View_Structure_h__