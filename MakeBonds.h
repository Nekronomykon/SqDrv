#ifndef Make_Bonds_h__
#define Make_Bonds_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkSimpleBondPerceiver.h>

using MakeBonds = vtkSimpleBondPerceiver;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<MakeBonds>           NewMakeBonds;
typedef vtkSmartPointer<MakeBonds>   ToMakeBonds;

#endif // !Make_Bonds_h__

