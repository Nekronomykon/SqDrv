#ifndef Export_File_PS_h__
#define Export_File_PS_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FormatFile.h"

#include <vtkPostScriptWriter.h>

using ExportFilePostScript = vtkPostScriptWriter;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<ExportFilePostScript>            NewExportPS;
typedef vtkSmartPointer<ExportFilePostScript>    ToExportPS;

template <class Host>
bool ExportToPostScriptFile(Host &host, Path a_path)
{
  NewExportPS writer;
  writer->SetFileName(a_path.c_str());
  return host.getMoleculeView()->exportImageTo(writer, false);
}

#endif // !Export_File_PS_h__
