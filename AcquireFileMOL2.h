#ifndef AcquireFile_MOL2_h__
#define AcquireFile_MOL2_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FormatFile.h"
#include "AcquireFileBase.h"

#include <vtkNew.h>
#include <vtkSmartPointer.h>

class AcquireFileMOL2
: public AcquireFileBase
{};

typedef vtkNew<AcquireFileMOL2> NewAcquireMOL2;
typedef vtkSmartPointer<AcquireFileMOL2> ToAcquireMOL2;



#endif // !AcquireFile_MOL2_h__

