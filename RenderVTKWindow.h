#ifndef Render_VTK_Window_h__
#define Render_VTK_Window_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <memory>

#include <vtkRenderWindow.h>

class RenderVTKWindow
{
public:
  explicit RenderVTKWindow(vtkRenderWindow * /*p*/ = nullptr);
  ~RenderVTKWindow();
  //
  vtkRenderWindow *reset(vtkRenderWindow * /*p*/ = nullptr);
  //
  operator vtkRenderWindow *() const { return ptr_; }
  vtkRenderWindow *operator->() const { return ptr_; }

private:
  vtkRenderWindow *ptr_;
};

#endif // !Render_VTK_Window_h__
