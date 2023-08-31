#include "SourceOfMolecule.h"

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    SourceOfMolecule.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <vtkCommand.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkTrivialProducer.h>

using namespace std;

//------------------------------------------------------------------------------
vtkStandardNewMacro(SourceOfMolecule);

//------------------------------------------------------------------------------
SourceOfMolecule::SourceOfMolecule(int nOuts)
{
  // by default assume filters have one input and one output
  // subclasses that deviate should modify this setting
  // this->SetNumberOfInputPorts(1);
  this->SetNumberOfOutputPorts(nOuts);
}

//------------------------------------------------------------------------------
// SourceOfMolecule::~SourceOfMolecule();

//------------------------------------------------------------------------------
void SourceOfMolecule::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
Molecule *SourceOfMolecule::GetOutput()
{
  return this->GetOutput(0);
}

//------------------------------------------------------------------------------
Molecule *SourceOfMolecule::GetOutput(int port)
{
  return Molecule::SafeDownCast(this->GetOutputDataObject(port));
}

//------------------------------------------------------------------------------
void SourceOfMolecule::SetOutput(Molecule *d)
{
  this->GetExecutive()->SetOutputData(0, d);
}

//------------------------------------------------------------------------------
vtkDataObject *SourceOfMolecule::GetInput()
{
  return this->GetInput(0);
}

//------------------------------------------------------------------------------
vtkDataObject *SourceOfMolecule::GetInput(int port)
{
  return this->GetExecutive()->GetInputData(port, 0);
}

//------------------------------------------------------------------------------
Molecule *SourceOfMolecule::GetMoleculeInput(int port)
{
  return Molecule::SafeDownCast(this->GetInput(port));
}

//------------------------------------------------------------------------------
vtkTypeBool SourceOfMolecule::ProcessRequest(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector)
{
  // generate the data
  if (request->Has(vtkDemandDrivenPipeline::REQUEST_DATA()))
  {
    return this->RequestData(request, inputVector, outputVector);
  }

  if (request->Has(vtkStreamingDemandDrivenPipeline::REQUEST_UPDATE_EXTENT()))
  {
    return this->RequestUpdateExtent(request, inputVector, outputVector);
  }

  // execute information
  if (request->Has(vtkDemandDrivenPipeline::REQUEST_INFORMATION()))
  {
    return this->RequestInformation(request, inputVector, outputVector);
  }

  return this->Superclass::ProcessRequest(request, inputVector, outputVector);
}

//------------------------------------------------------------------------------
int SourceOfMolecule::FillOutputPortInformation(int vtkNotUsed(port), vtkInformation *info)
{
  // now add our info
  info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkMolecule");
  return 1;
}

//------------------------------------------------------------------------------
int SourceOfMolecule::FillInputPortInformation(int vtkNotUsed(port), vtkInformation *info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkMolecule");
  return 1;
}

//------------------------------------------------------------------------------
int SourceOfMolecule::RequestInformation(vtkInformation *vtkNotUsed(request),
                                       vtkInformationVector **vtkNotUsed(inputVector),
                                       vtkInformationVector *vtkNotUsed(outputVector))
{
  // do nothing let subclasses handle it
  return 1;
}

//------------------------------------------------------------------------------
int SourceOfMolecule::RequestUpdateExtent(vtkInformation *vtkNotUsed(request),
                                        vtkInformationVector **inputVector,
                                        vtkInformationVector *vtkNotUsed(outputVector))
{
  int numInputPorts = this->GetNumberOfInputPorts();
  for (int i = 0; i < numInputPorts; i++)
  {
    int numInputConnections = this->GetNumberOfInputConnections(i);
    for (int j = 0; j < numInputConnections; j++)
    {
      vtkInformation *inputInfo = inputVector[i]->GetInformationObject(j);
      inputInfo->Set(vtkStreamingDemandDrivenPipeline::EXACT_EXTENT(), 1);
    }
  }
  return 1;
}

//------------------------------------------------------------------------------
// This is the superclasses style of Execute method.  Convert it into
// an imaging style Execute method.
int SourceOfMolecule::RequestData(vtkInformation *vtkNotUsed(request),
                                vtkInformationVector **vtkNotUsed(inputVector),
                                vtkInformationVector *vtkNotUsed(outputVector))
{
  return 1;
}

//------------------------------------------------------------------------------
void SourceOfMolecule::SetInputData(vtkDataObject *input)
{
  this->SetInputData(0, input);
}

//------------------------------------------------------------------------------
void SourceOfMolecule::SetInputData(int index, vtkDataObject *input)
{
  this->SetInputDataInternal(index, input);
}

//------------------------------------------------------------------------------
void SourceOfMolecule::AddInputData(vtkDataObject *input)
{
  this->AddInputData(0, input);
}

//------------------------------------------------------------------------------
void SourceOfMolecule::AddInputData(int index, vtkDataObject *input)
{

  this->AddInputDataInternal(index, input);
}
