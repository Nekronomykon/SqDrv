#ifndef Choose_ColorByName_h__
#define Choose_ColorByName_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QComboBox>

#include <QString>
#include <QStringList>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include <vtkColor.h>
#include <vtkStdString.h>
#include <vtkNamedColors.h>

class QVTKNamedColors
    : public QComboBox
{
  Q_OBJECT
public:
  explicit QVTKNamedColors(QWidget * /*parent*/ = nullptr);

  static void mapColor(const char* key, vtkColor3d& color);

private:
  static QStringList ColorNames();
    static vtkNew<vtkNamedColors> colorsNamed;

};

#endif // !Choose_ColorByName_h__

