#ifndef Export_File_BMP_h__
#define Export_File_BMP_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FormatFile.h"

#include <vtkBMPWriter.h>

using ExportFileBMP = vtkBMPWriter;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<ExportFileBMP>            NewExportBMP;
typedef vtkSmartPointer<ExportFileBMP>    ToExportBMP;

template <class Host>
bool ExportToBMPFile(Host &host, Path a_path)
{
  NewExportBMP writer;
  writer->SetFileName(a_path.c_str());
  return host.getMoleculeView()->exportImageTo(writer, true);
}

#endif // !Export_File_BMP_h__
