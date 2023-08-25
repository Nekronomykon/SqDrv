#include "ViewMolecule.h"

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleRubberBandPick.h>

#include <vtkWindowToImageFilter.h>
#include <vtkProperty.h>
#include <vtkCamera.h>

#include <vtkNamedColors.h>

#include "RenderVTKWindow.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
ViewMolecule::ViewMolecule(QWidget *parent)
    : QVTKRenderWidget(parent),
      mapMol_(AMolMap::New()),
      colorBackground_(0.750, 0.625, 0.875)
{
  // I do not know, whethet this practice is meant as the good one,
  // or even appropriate, for the future of VTK @ Qt:
  vtkRenderWindow *pRW = this->renderWindow();
  pRW->SetNumberOfLayers(2);
  //
  RWInteractor *pRWI = this->interactor();
  while (!pRWI)
  {
    NewRWInteractor new_rwi;
    // NewQVTKInteractor new_rwi;
    pRW->SetInteractor(new_rwi);
    pRWI = this->interactor();
  }
  pRWI->SetInteractorStyle(insStr_);
  // pRWI->SetInteractorStyle(StyleInteractionCamera::New());
  // The InteractorStyle mechanism is the 3D GUI mechanism to be used in editing,
  // So we would try to override it by our own mechanism... yet not now.
  // In progress. Under the development.
  //
  // renderBg_->SetBackground(colorBackground_.GetData());
  // mapMol_->SetInputConnection(molecule_);
  mapMol_->UseFastSettings();
  this->buildView();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void ViewMolecule::buildView()
{
  RenderVTKWindow pRW(this->renderWindow());
  // Render BG:
  pRW->RemoveRenderer(renderBg_);
  renderBg_->SetBackground(colorBackground_.GetData());
  renderBg_->SetLayer(0);
  pRW->AddRenderer(renderBg_);
  //
  pRW->RemoveRenderer(renderMol_);
  renderMol_->RemoveActor(actorMol_);
  // -----------------------------------------------------------
  // read these properties from registry?
  actorMol_->GetProperty()->SetDiffuse(0.75);
  actorMol_->GetProperty()->SetSpecular(0.5);
  actorMol_->GetProperty()->SetSpecularPower(20.0);
  // -----------------------------------------------------------
  actorMol_->SetMapper(this->getMolMap());
  renderMol_->AddActor(actorMol_);
  renderMol_->SetLayer(1);
  pRW->AddRenderer(renderMol_);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
vtkColor3d &ViewMolecule::colorBackground(void)
{
  return colorBackground_;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
vtkColor3d ViewMolecule::getColorBackground(void) const
{
  return colorBackground_;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
double ViewMolecule::backgroundRed(void) const
{
  return colorBackground_.GetRed();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
double ViewMolecule::backgroundGreen(void) const
{
  return colorBackground_.GetGreen();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
double ViewMolecule::backgroundBlue(void) const
{
  return colorBackground_.GetBlue();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void ViewMolecule::setColorBackground(const vtkColor3d &newColorBackground)
{
  RenderVTKWindow rw(this->renderWindow());
  colorBackground_ = newColorBackground;
  renderBg_->SetBackground(colorBackground_.GetData());
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void ViewMolecule::setColorBackground(double r, double g, double b)
{
  RenderVTKWindow rw(this->renderWindow());
  colorBackground_.Set(r, g, b);
  renderBg_->SetBackground(colorBackground_.GetData());
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
vtkColor3d ViewMolecule::resetColorBackground(vtkColor3d color)
{
  RenderVTKWindow rw(this->renderWindow());
  std::swap(colorBackground_, color);
  renderBg_->SetBackground(colorBackground_.GetData());
  return color;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void ViewMolecule::setBackgroundRed(double v)
{
  RenderVTKWindow rw(this->renderWindow());
  colorBackground_.SetRed(v);
  renderBg_->SetBackground(colorBackground_.GetData());
  this->buildView();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void ViewMolecule::setBackgroundGreen(double v)
{
  RenderVTKWindow rw(this->renderWindow());
  colorBackground_.SetGreen(v);
  renderBg_->SetBackground(colorBackground_.GetData());
  this->buildView();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void ViewMolecule::setBackgroundBlue(double v)
{
  RenderVTKWindow rw(this->renderWindow());
  colorBackground_.SetBlue(v);
  renderBg_->SetBackground(colorBackground_.GetData());
  this->buildView();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void ViewMolecule::ColorBackgroundByName(vtkStdString sName)
{
  RenderVTKWindow rw(this->renderWindow());
  vtkNew<vtkNamedColors> pColors;
  pColors->GetColor(sName, colorBackground_);
  renderBg_->SetBackground(colorBackground_.GetData());
  this->buildView();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
Renderer *ViewMolecule::getRenderBack(void) const
{
  return renderBg_;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
Renderer *ViewMolecule::getRenderMol(void) const
{
  return renderMol_;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool ViewMolecule::exportImageTo(ImageWriter *pIW, bool bAlpha)
{
  if (!pIW)
    return false;
  //
  RenderVTKWindow pRW(this->renderWindow());
  //
  vtkNew<vtkWindowToImageFilter> w2if;
  w2if->SetFixBoundary(true);
  // w2if->SetScale(4); --> how to scale label fonts???  to think on...
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
  return true;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
ViewMolecule *ViewMolecule::resetCamera(void)
{
  RenderVTKWindow pRW(this->renderWindow());
  vtkRenderer *pRen = this->getRenderMol();
  if (pRen)
    pRen->ResetCamera();
  pRW->Modified();
  return this;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
ViewMolecule *ViewMolecule::setProjectParallel(bool bReset)
{
  RenderVTKWindow pRW(this->renderWindow());
  vtkRenderer *pRen = this->getRenderMol();
  vtkCamera *pCam = !pRen ? nullptr : pRen->GetActiveCamera();
  if (pCam && !pCam->GetParallelProjection())
    pCam->ParallelProjectionOn();
  else
  {
    if (pRen && bReset)
      pRen->ResetCamera();
  }
  pRW->Modified();
  return this;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
ViewMolecule *ViewMolecule::setProjectPerspective(bool bReset)
{
  RenderVTKWindow pRW(this->renderWindow());
  vtkRenderer *pRen = this->getRenderMol();
  vtkCamera *pCam = pRen->GetActiveCamera();
  if (pCam && !pCam->GetParallelProjection())
    pCam->ParallelProjectionOff();
  else
  {
    if (pRen && bReset)
      pRen->ResetCamera();
  }
  pRW->Modified();
  return this;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
MapMolecule *ViewMolecule::getMolMap() const
{
  return mapMol_;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
Molecule *ViewMolecule::getMolecule(void) const
{
  return mapMol_->GetInput();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void ViewMolecule::mapMolecule(Molecule *pMol)
{
  if (pMol && pMol != this->getMolecule())
    mapMol_->SetInputData(pMol);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void ViewMolecule::showMolecule(Molecule *pMol, bool bResetCamera)
{
  RenderVTKWindow pRW(this->renderWindow());
  if (pMol && pMol != this->getMolecule())
    this->getMolecule()->DeepCopy(pMol);
  if (bResetCamera)
    renderMol_->ResetCamera();
  this->buildView();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
ViewMolecule *ViewMolecule::setStyleFast()
{
  mapMol_->UseFastSettings();
  return this;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
ViewMolecule *ViewMolecule::setStyleBall()
{
  mapMol_->UseBallAndStickSettings();
  return this;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
ViewMolecule *ViewMolecule::setStyleStick()
{
  mapMol_->UseLiquoriceStickSettings();
  return this;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
ViewMolecule *ViewMolecule::setStyleSpace()
{
  mapMol_->UseVDWSpheresSettings();
  return this;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
