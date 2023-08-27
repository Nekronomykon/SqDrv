#ifndef Export_File_PNG_h__
#define Export_File_PNG_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FormatFile.h"

#include <vtkPNGWriter.h>

using ExportFilePNG = vtkPNGWriter;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<ExportFilePNG>            NewExportPNG;
typedef vtkSmartPointer<ExportFilePNG>    ToExportPNG;

template <class Host>
bool ExportToPNGFile(Host &host, Path a_path)
{
  NewExportPNG writer;
  writer->SetFileName(a_path.c_str());
  return host.getMoleculeView()->exportImageTo(writer, true);
}

#endif // !Export_File_PNG_h__
