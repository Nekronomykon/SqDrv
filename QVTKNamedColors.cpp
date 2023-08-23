#include "QVTKNamedColors.h"

#include <vtkNamedColors.h>

vtkNew<vtkNamedColors> QVTKNamedColors::colorsNamed;

QStringList QVTKNamedColors::ColorNames()
{
  static QStringList allNames;

  if (allNames.isEmpty())
  {
    vtkNew<vtkStringArray> col_names;
    colorsNamed->GetColorNames(col_names);
    size_t nNames = col_names->GetSize();
    for (size_t j = 0; j < nNames; ++j)
    {
      vtkStdString a_name(col_names->GetValue(j));
      if (a_name.empty())
        continue;
      QString sName(tr(a_name.c_str()));
      if (!sName.isEmpty())
        allNames << sName;
    }
  }

  return allNames;
}

QVTKNamedColors::QVTKNamedColors(QWidget *parent)
    : QComboBox(parent)
{
  this->addItem(tr("[: user color :]")); // placeholder
  this->addItems(ColorNames());
}

void QVTKNamedColors::mapColor(const char* key, vtkColor3d& color)
{
  colorsNamed->GetColor(key,color);
}