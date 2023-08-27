#ifndef Export_File_TIFF_h__
#define Export_File_TIFF_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FormatFile.h"

#include <vtkTIFFWriter.h>

using ExportFileTIFF = vtkTIFFWriter;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<ExportFileTIFF>           NewExportTIFF;
typedef vtkSmartPointer<ExportFileTIFF>   ToExportTIFF;

template <class Host>
bool ExportToTIFFFile(Host &host, Path a_path)
{
  NewExportTIFF writer;
  writer->SetFileName(a_path.c_str());
  return host.getMoleculeView()->exportImageTo(writer, true);
}

#endif // !Export_File_TIFF_h__
