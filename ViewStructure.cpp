#include "ViewStructure.h"

#include <vtkRenderWindow.h>
#include <vtkWindowToImageFilter.h>

#include <vtkCamera.h>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleRubberBandPick.h>

#include <QString>
#include <QMessageBox>

#include "MapMoleculeOpenGL.h"

#include "ResetCursor.h"

typedef vtkNew<vtkRenderWindowInteractor> NewRenderWindowInteractor;
typedef vtkSmartPointer<vtkRenderWindowInteractor> ARenderWindowInteractor;

///////////////////////////////////////////////////////////////////////////////
//
//
ViewStructure::ViewStructure(QWidget *parent)
    : QVTKOpenGLNativeWidget(parent),         //
      styleMol_(StyleMapMolecule::styleFast), //
      mapMol_(MoleculeMapper::New())
// mapMol_(MapMoleculeOpenGL::New())          // is it constant?
{
    actorMol_->SetMapper(mapMol_);

    // initializing window mode:
    vtkRenderWindow *pRW = this->renderWindow();
    pRW->SetNumberOfLayers(2);

    colorBg_.SetRed(1.0);   // debug colors
    colorBg_.SetGreen(1.0); // total up to yellow
    colorBg_.SetBlue(0.75); // light-yellow

    // general rendering setup
    this->initRendering();
}
//
///////////////////////////////////////////////////////////////////////////////
//
vtkColor3d ViewStructure::getBgColor() const
{
    return colorBg_;
}
//
///////////////////////////////////////////////////////////////////////////////
//
vtkColor3d &ViewStructure::BgColor()
{
    return colorBg_;
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool ViewStructure::initRendering(Molecule *pMol)
{
    vtkRenderWindow *pRW = this->renderWindow();

    vtkRenderWindowInteractor *pRWI = this->interactor();

    vtkSmartPointer<vtkInteractorStyleTrackballCamera>
        istyle_rb(vtkInteractorStyleRubberBandPick::New()); // see the song...
    // The InteractorStyle mechanism is the 3D GUI mechanism to be used in editing,
    // So we would try to override it by our own mechanism...
    if (!pRWI)
    {
        NewRenderWindowInteractor new_rwi;
        pRW->SetInteractor(new_rwi);
        pRWI = new_rwi.Get();
    }
    pRWI->SetInteractorStyle(istyle_rb);

    // [0] BACKGROUND
    renderBg_->SetLayer(0);
    renderBg_->SetBackground(colorBg_.GetData());
    pRW->AddRenderer(renderBg_);

    // [1] MOLECULE
    // reset molecular rendering:
    renderMol_->RemoveActor(actorMol_);
    pRW->RemoveRenderer(renderMol_); // detached:

    if (pMol)
    { // if molecule is valid to render (nullptr is to clear most of parameters)
        styleMol_.SetupMapMolecule(mapMol_.Get());
    }
    mapMol_->SetInputData(pMol);

    // rebuilding:
    renderMol_->SetLayer(1);
    renderMol_->AddActor(actorMol_);
    // renderMol_->SetBackground(colorBg_.GetData());
    pRW->AddRenderer(renderMol_);
    return true;
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool ViewStructure::resetMoleculeStyle(const StyleMapMolecule &style)
{
    bool bChanged(styleMol_ != style);
    if (bChanged)
    {
        styleMol_ = style;
        styleMol_.SetupMapMolecule(mapMol_.Get());
    }
    return bChanged;
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool ViewStructure::exportImageTo(vtkImageWriter *pIW, bool bAlpha)
{
    if (!pIW)
        return false;
    //
    vtkRenderWindow *pRW = this->renderWindow();
    if (!pRW)
        return false;
    pRW->Render();
    //
    vtkNew<vtkWindowToImageFilter> w2if;
    w2if->SetFixBoundary(true);
    w2if->SetScale(4);
    //
    w2if->SetInput(pRW);
    if (bAlpha)
        w2if->SetInputBufferTypeToRGBA();
    else
        w2if->SetInputBufferTypeToRGB();
    w2if->ReadFrontBufferOff();
    w2if->Update();

    pIW->SetInputConnection(w2if->GetOutputPort());
    pIW->Write();
    //
    pRW->Modified();
    return true;
}
//
///////////////////////////////////////////////////////////////////////////////
//
void ViewStructure::ProjectParallel()
{
    vtkRenderWindow *pRW = this->renderWindow();
    // pRW->RemoveRenderer(renderMol_);
    renderMol_->GetActiveCamera()->ParallelProjectionOn();
    // pRW->AddRenderer(renderMol_);
    // renderMol_->Render();
    pRW->Modified();
    // pRW->Render();
}
//
///////////////////////////////////////////////////////////////////////////////
//
void ViewStructure::ProjectPerspective()
{
    vtkRenderWindow *pRW = this->renderWindow();
    // pRW->RemoveRenderer(renderMol_);
    renderMol_->GetActiveCamera()->ParallelProjectionOff();
    // pRW->AddRenderer(renderMol_);
    // renderMol_->Render();
    pRW->Modified();
    // pRW->Render();
}
//
///////////////////////////////////////////////////////////////////////////////
