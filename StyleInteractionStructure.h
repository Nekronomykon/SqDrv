#ifndef Style_Interaction_Structure_h__
#define Style_Interaction_Structure_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

// #include "vtkInteractionStyleModule.h" // For export macro
#include "StyleInteractionCamera.h"

#include <vtkNew.h>

class vtkUnsignedCharArray;

class /* VTKINTERACTIONSTYLE_EXPORT */ StyleInteractionStructure
    : public StyleInteractionCamera
{
public:
  static StyleInteractionStructure *New();
  vtkTypeMacro(StyleInteractionStructure, StyleInteractionCamera);
  void PrintSelf(ostream &os, vtkIndent indent) override;

  void StartSelect();

  ///@{
  /**
   * Event bindings
   */
  void OnMouseMove() override;
  void OnLeftButtonDown() override;
  void OnLeftButtonUp() override;
  void OnChar() override;
  ///@}

protected:
  StyleInteractionStructure();
  ~StyleInteractionStructure() override;

  virtual void Pick();
  void RedrawRubberBand();

  int StartPosition[2] = {0, 0};
  int EndPosition[2] = {0, 0};

  int Moving = 0;

  vtkNew<vtkUnsignedCharArray> PixelArray;

private:
  StyleInteractionStructure(const StyleInteractionStructure &) = delete;
  void operator=(const StyleInteractionStructure &) = delete;
};

#endif // !Style_Interaction_Structure_h__
