#ifndef Style_Interaction_Camera_h__
#define Style_Interaction_Camera_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

// #include "vtkInteractionStyleModule.h" // For export macro
#include <vtkInteractorStyle.h>

class /* VTKINTERACTIONSTYLE_EXPORT */ StyleInteractionCamera : public vtkInteractorStyle
{
public:
  static StyleInteractionCamera *New();
  vtkTypeMacro(StyleInteractionCamera, vtkInteractorStyle);
  void PrintSelf(ostream &os, vtkIndent indent) override;

  ///@{
  /**
   * Event bindings controlling the effects of pressing mouse buttons
   * or moving the mouse.
   */
  void OnMouseMove() override;
  void OnLeftButtonDown() override;
  void OnLeftButtonUp() override;
  void OnMiddleButtonDown() override;
  void OnMiddleButtonUp() override;
  void OnRightButtonDown() override;
  void OnRightButtonUp() override;
  void OnMouseWheelForward() override;
  void OnMouseWheelBackward() override;
  ///@}

  // These methods for the different interactions in different modes
  // are overridden in subclasses to perform the correct motion. Since
  // they are called by OnTimer, they do not have mouse coord parameters
  // (use interactor's GetEventPosition and GetLastEventPosition)
  void Rotate() override;
  void Spin() override;
  void Pan() override;
  void Dolly() override;
  void EnvironmentRotate() override;

  ///@{
  /**
   * Set the apparent sensitivity of the interactor style to mouse motion.
   */
  vtkSetMacro(MotionFactor, double);
  vtkGetMacro(MotionFactor, double);
  ///@}
  enum
  {
    InteractionModeOrient = 0,
    InteractionModeSelect = 1,
    InteractionModeChange = 2
  };
  int CurrentMode = InteractionModeOrient;

  int GetCurrentMode(void)const;
  int SetCurrentMode(int mode);


protected:
  StyleInteractionCamera();
  ~StyleInteractionCamera() override;

  double MotionFactor = 10.0;

  virtual void Dolly(double factor);

private:
  StyleInteractionCamera(const StyleInteractionCamera &) = delete;
  void operator=(const StyleInteractionCamera &) = delete;
};

#endif // !Style_Interaction_Camera_h__
