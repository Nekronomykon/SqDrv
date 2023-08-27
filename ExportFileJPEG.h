#ifndef Export_File_JPEG_h__
#define Export_File_JPEG_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FormatFile.h"

#include <vtkJPEGWriter.h>

using ExportFileJPEG = vtkJPEGWriter;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<ExportFileJPEG>           NewExportJPEG;
typedef vtkSmartPointer<ExportFileJPEG>   ToExportJPEG;

template <class Host>
bool ExportToJPEGFile(Host &host, Path a_path)
{
  NewExportJPEG writer;
  writer->SetFileName(a_path.c_str());
  return host.getMoleculeView()->exportImageTo(writer, false);
}

#endif // !Export_File_JPEG_h__
