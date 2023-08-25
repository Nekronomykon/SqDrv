#ifndef View_Molecule_h__
#define View_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include <vtkColor.h>

///////////////////////////////////////////////////////////////////////////
//
#include <vtkRenderer.h>
using Renderer = vtkRenderer;
typedef vtkNew<Renderer> NewRenderer;
typedef vtkSmartPointer<Renderer> ARenderer;
//
#include <vtkImageWriter.h>
using ImageWriter = vtkImageWriter;
typedef vtkNew<ImageWriter> NewImageWriter;
typedef vtkSmartPointer<ImageWriter> AnImageWriter;
//
#include <vtkActor.h>
using Actor = vtkActor;
typedef vtkNew<Actor> NewActor;
typedef vtkSmartPointer<Actor> AnActor;
//
#include <vtkLODActor.h>
using LODActor = vtkLODActor;
typedef vtkNew<LODActor> NewLODActor;
typedef vtkSmartPointer<LODActor> ALODActor;
//
#include <vtkRenderWindowInteractor.h>
using RWInteractor = vtkRenderWindowInteractor;
typedef vtkNew<RWInteractor> NewRWInteractor;
typedef vtkSmartPointer<RWInteractor> AnRWInteractor;
//
#include <QVTKInteractor.h>
typedef vtkNew<QVTKInteractor> NewQVTKInteractor;
typedef vtkSmartPointer<QVTKInteractor> AQVTKInteractor;
//
#include <vtkInteractorStyle.h>
using InStyle = vtkInteractorStyle;
typedef vtkNew<InStyle> NewInStyle;
typedef vtkSmartPointer<InStyle> AnInStyle;
//
#include <vtkMolecule.h>
using Molecule = vtkMolecule;
typedef vtkNew<Molecule> NewMolecule;
typedef vtkSmartPointer<Molecule> AMolecule;
//
#include "MapMolecule.h"
// using MapMolecule = vtkMoleculeMapper;
typedef vtkNew<MapMolecule> NewMolMap;
typedef vtkSmartPointer<MapMolecule> AMolMap;
#include "StyleMapMolecule.h"
#include "StyleInteractionStructure.h"
typedef vtkNew<StyleInteractionStructure> NewInStyleStructure;
//
//
///////////////////////////////////////////////////////////////////////////
#include <QVTKRenderWidget.h>

class ViewMolecule : public QVTKRenderWidget
{
public:
  explicit ViewMolecule(QWidget * /*parent*/ = nullptr);
  //
  vtkColor3d getColorBackground(void) const;
  vtkColor3d &colorBackground(void);
  double backgroundRed(void) const;
  double backgroundGreen(void) const;
  double backgroundBlue(void) const;
  void setColorBackground(const vtkColor3d & /*color*/);
  void setColorBackground(double r, double g, double b);
  vtkColor3d resetColorBackground(vtkColor3d /*color*/);
  void setBackgroundRed(double v);
  void setBackgroundGreen(double v);
  void setBackgroundBlue(double v);
  void ColorBackgroundByName(vtkStdString /*sName*/);
  //
  Renderer *getRenderBack(void) const;
  Renderer *getRenderMol(void) const;
  //
  bool exportImageTo(ImageWriter * /*pIW*/, bool /*bAlpha*/ = true);
  //
  ViewMolecule *resetCamera(void);
  ViewMolecule *setProjectParallel(bool /* bResetCameraIfAlreadySetSo */ = false);
  ViewMolecule *setProjectPerspective(bool /* bResetCameraIfAlreadySetSo */ = false);
  //
  MapMolecule *getMolMap(void) const;
  Molecule *getMolecule(void) const;
  //
  void mapMolecule(Molecule * /*pMol*/);            //
  void showMolecule(Molecule * /*pMol*/ = nullptr,  //
                    bool /*bResetCamera*/ = false); // false <-- final version
                                                    //  true <-- debug version
                                                    //
  void buildView(void);
  //
  ViewMolecule *setStyleFast(void);
  ViewMolecule *setStyleBall(void);
  ViewMolecule *setStyleSpace(void);
  ViewMolecule *setStyleStick(void);

protected:
  //
private:
  NewInStyleStructure insStr_;
  // bg
  vtkColor3d colorBackground_;
  NewRenderer renderBg_;
  // mol
  NewActor actorMol_;
  NewRenderer renderMol_;
  AMolMap mapMol_;
};

#endif // !View_Molecule_h__
