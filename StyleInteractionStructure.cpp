#include "StyleInteractionStructure.h"

#include <vtkAbstractPropPicker.h>
#include <vtkAreaPicker.h>
#include <vtkAssemblyPath.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkUnsignedCharArray.h>

vtkStandardNewMacro(StyleInteractionStructure);

//------------------------------------------------------------------------------
StyleInteractionStructure::StyleInteractionStructure()
    : Moving(0)
{
}

//------------------------------------------------------------------------------
StyleInteractionStructure::~StyleInteractionStructure() = default;

//------------------------------------------------------------------------------
void StyleInteractionStructure::StartSelect()
{
  this->CurrentMode = InteractionModeSelect;
}

//------------------------------------------------------------------------------
void StyleInteractionStructure::OnChar()
{
  switch (this->Interactor->GetKeyCode())
  {
  case 'r':
  case 'R':
    // r toggles the rubber band selection mode for mouse button 1
    if (this->CurrentMode == InteractionModeOrient)
    {
      this->CurrentMode = InteractionModeSelect;
    }
    else
    {
      this->CurrentMode = InteractionModeOrient;
    }
    break;
  case 'p':
  case 'P':
  {
    vtkRenderWindowInteractor *rwi = this->Interactor;
    int *eventPos = rwi->GetEventPosition();
    this->FindPokedRenderer(eventPos[0], eventPos[1]);
    this->StartPosition[0] = eventPos[0];
    this->StartPosition[1] = eventPos[1];
    this->EndPosition[0] = eventPos[0];
    this->EndPosition[1] = eventPos[1];
    this->Pick();
    break;
  }
  default:
    this->Superclass::OnChar();
  }
}

//------------------------------------------------------------------------------
void StyleInteractionStructure::OnLeftButtonDown()
{
  if (this->CurrentMode == InteractionModeOrient)
  {
    // the base class is responsible for the orientation
    this->Superclass::OnLeftButtonDown();
    return;
  }

  if (!this->Interactor)
    return;

  // otherwise record the rubber band starting coordinate

  if (this->CurrentMode == InteractionModeSelect)
  {
    this->Moving = 1;

    vtkRenderWindow *renWin = this->Interactor->GetRenderWindow();

    this->StartPosition[0] = this->Interactor->GetEventPosition()[0];
    this->StartPosition[1] = this->Interactor->GetEventPosition()[1];
    this->EndPosition[0] = this->StartPosition[0];
    this->EndPosition[1] = this->StartPosition[1];

    this->PixelArray->Initialize();
    this->PixelArray->SetNumberOfComponents(4);
    const int *size = renWin->GetSize();
    this->PixelArray->SetNumberOfTuples(size[0] * size[1]);

    renWin->GetRGBACharPixelData(0, 0, size[0] - 1, size[1] - 1, 1, this->PixelArray);

    this->FindPokedRenderer(this->StartPosition[0], this->StartPosition[1]);
  }
}

//------------------------------------------------------------------------------
void StyleInteractionStructure::OnMouseMove()
{
  if (this->CurrentMode == InteractionModeOrient)
  {
    // the base class is responsible for the orientation
    this->Superclass::OnMouseMove();
    return;
  }

  if (!this->Interactor || !this->Moving)
    return;

  if (this->CurrentMode == InteractionModeSelect)
  {
    this->EndPosition[0] = this->Interactor->GetEventPosition()[0];
    this->EndPosition[1] = this->Interactor->GetEventPosition()[1];
    const int *size = this->Interactor->GetRenderWindow()->GetSize();
    if (this->EndPosition[0] > (size[0] - 1))
    {
      this->EndPosition[0] = size[0] - 1;
    }
    if (this->EndPosition[0] < 0)
    {
      this->EndPosition[0] = 0;
    }
    if (this->EndPosition[1] > (size[1] - 1))
    {
      this->EndPosition[1] = size[1] - 1;
    }
    if (this->EndPosition[1] < 0)
    {
      this->EndPosition[1] = 0;
    }
    this->RedrawRubberBand();
  }
}

//------------------------------------------------------------------------------
void StyleInteractionStructure::OnLeftButtonUp()
{
  if (this->CurrentMode == InteractionModeOrient)
  {
    // the base class is responsible for the orientation
    this->Superclass::OnLeftButtonUp();
    return;
  }

  if (!this->Interactor || !this->Moving)
    return;

  if (this->CurrentMode == InteractionModeSelect)
  { // otherwise record the rubber band end coordinate and then fire off a pick
    if ((this->StartPosition[0] != this->EndPosition[0]) ||
        (this->StartPosition[1] != this->EndPosition[1]))
    {
      this->Pick();
    }
    this->Moving = 0;
  } // this->CurrentMode = VTKISRBP_ORIENT;
}

//------------------------------------------------------------------------------
void StyleInteractionStructure::RedrawRubberBand()
{
  // update the rubber band on the screen
  const int *size = this->Interactor->GetRenderWindow()->GetSize();

  vtkUnsignedCharArray *tmpPixelArray = vtkUnsignedCharArray::New();
  tmpPixelArray->DeepCopy(this->PixelArray);
  unsigned char *pixels = tmpPixelArray->GetPointer(0);

  int min[2], max[2];

  min[0] =
      this->StartPosition[0] <= this->EndPosition[0] ? this->StartPosition[0] : this->EndPosition[0];
  if (min[0] < 0)
  {
    min[0] = 0;
  }
  if (min[0] >= size[0])
  {
    min[0] = size[0] - 1;
  }

  min[1] =
      this->StartPosition[1] <= this->EndPosition[1] ? this->StartPosition[1] : this->EndPosition[1];
  if (min[1] < 0)
  {
    min[1] = 0;
  }
  if (min[1] >= size[1])
  {
    min[1] = size[1] - 1;
  }

  max[0] =
      this->EndPosition[0] > this->StartPosition[0] ? this->EndPosition[0] : this->StartPosition[0];
  if (max[0] < 0)
  {
    max[0] = 0;
  }
  if (max[0] >= size[0])
  {
    max[0] = size[0] - 1;
  }

  max[1] =
      this->EndPosition[1] > this->StartPosition[1] ? this->EndPosition[1] : this->StartPosition[1];
  if (max[1] < 0)
  {
    max[1] = 0;
  }
  if (max[1] >= size[1])
  {
    max[1] = size[1] - 1;
  }

  int i;
  for (i = min[0]; i <= max[0]; i++)
  {
    pixels[4 * (min[1] * size[0] + i)] = 255 ^ pixels[4 * (min[1] * size[0] + i)];
    pixels[4 * (min[1] * size[0] + i) + 1] = 255 ^ pixels[4 * (min[1] * size[0] + i) + 1];
    pixels[4 * (min[1] * size[0] + i) + 2] = 255 ^ pixels[4 * (min[1] * size[0] + i) + 2];
    pixels[4 * (max[1] * size[0] + i)] = 255 ^ pixels[4 * (max[1] * size[0] + i)];
    pixels[4 * (max[1] * size[0] + i) + 1] = 255 ^ pixels[4 * (max[1] * size[0] + i) + 1];
    pixels[4 * (max[1] * size[0] + i) + 2] = 255 ^ pixels[4 * (max[1] * size[0] + i) + 2];
  }
  for (i = min[1] + 1; i < max[1]; i++)
  {
    pixels[4 * (i * size[0] + min[0])] = 255 ^ pixels[4 * (i * size[0] + min[0])];
    pixels[4 * (i * size[0] + min[0]) + 1] = 255 ^ pixels[4 * (i * size[0] + min[0]) + 1];
    pixels[4 * (i * size[0] + min[0]) + 2] = 255 ^ pixels[4 * (i * size[0] + min[0]) + 2];
    pixels[4 * (i * size[0] + max[0])] = 255 ^ pixels[4 * (i * size[0] + max[0])];
    pixels[4 * (i * size[0] + max[0]) + 1] = 255 ^ pixels[4 * (i * size[0] + max[0]) + 1];
    pixels[4 * (i * size[0] + max[0]) + 2] = 255 ^ pixels[4 * (i * size[0] + max[0]) + 2];
  }

  this->Interactor->GetRenderWindow()->SetRGBACharPixelData(
      0, 0, size[0] - 1, size[1] - 1, pixels, 0);
  this->Interactor->GetRenderWindow()->Frame();

  tmpPixelArray->Delete();
}

//------------------------------------------------------------------------------
void StyleInteractionStructure::Pick()
{
  // find rubber band lower left, upper right and center
  double rbcenter[3];
  const int *size = this->Interactor->GetRenderWindow()->GetSize();
  int min[2], max[2];
  min[0] =
      this->StartPosition[0] <= this->EndPosition[0] ? this->StartPosition[0] : this->EndPosition[0];
  if (min[0] < 0)
  {
    min[0] = 0;
  }
  if (min[0] >= size[0])
  {
    min[0] = size[0] - 2;
  }

  min[1] =
      this->StartPosition[1] <= this->EndPosition[1] ? this->StartPosition[1] : this->EndPosition[1];
  if (min[1] < 0)
  {
    min[1] = 0;
  }
  if (min[1] >= size[1])
  {
    min[1] = size[1] - 2;
  }

  max[0] =
      this->EndPosition[0] > this->StartPosition[0] ? this->EndPosition[0] : this->StartPosition[0];
  if (max[0] < 0)
  {
    max[0] = 0;
  }
  if (max[0] >= size[0])
  {
    max[0] = size[0] - 2;
  }

  max[1] =
      this->EndPosition[1] > this->StartPosition[1] ? this->EndPosition[1] : this->StartPosition[1];
  if (max[1] < 0)
  {
    max[1] = 0;
  }
  if (max[1] >= size[1])
  {
    max[1] = size[1] - 2;
  }

  rbcenter[0] = (min[0] + max[0]) / 2.0;
  rbcenter[1] = (min[1] + max[1]) / 2.0;
  rbcenter[2] = 0;

  if (this->State == VTKIS_NONE)
  {
    // tell the RenderWindowInteractor's picker to make it happen
    vtkRenderWindowInteractor *rwi = this->Interactor;

    vtkAssemblyPath *path = nullptr;
    rwi->StartPickCallback();
    vtkAbstractPropPicker *picker = vtkAbstractPropPicker::SafeDownCast(rwi->GetPicker());
    if (picker != nullptr)
    {
      vtkAreaPicker *areaPicker = vtkAreaPicker::SafeDownCast(picker);
      if (areaPicker != nullptr)
      {
        areaPicker->AreaPick(min[0], min[1], max[0], max[1], this->CurrentRenderer);
      }
      else
      {
        picker->Pick(rbcenter[0], rbcenter[1], 0.0, this->CurrentRenderer);
      }
      path = picker->GetPath();
    }
    if (path == nullptr)
    {
      this->HighlightProp(nullptr);
      this->PropPicked = 0;
    }
    else
    {
      // highlight the one prop that the picker saved in the path
      // this->HighlightProp(path->GetFirstNode()->GetViewProp());
      this->PropPicked = 1;
    }
    rwi->EndPickCallback();
  }

  this->Interactor->Render();
}

//------------------------------------------------------------------------------
void StyleInteractionStructure::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
