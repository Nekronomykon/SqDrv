#include "ResultsFromMolecule.h"

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    ResultsFromMolecule.cxx

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
vtkStandardNewMacro(ResultsFromMolecule);

//------------------------------------------------------------------------------
ResultsFromMolecule::ResultsFromMolecule(int nOuts, int nIns)
{
  // by default assume filters have one input and one output
  // subclasses that deviate should modify this setting
  this->SetNumberOfInputPorts(nIns);
  this->SetNumberOfOutputPorts(nOuts);
}

//------------------------------------------------------------------------------
// ResultsFromMolecule::~ResultsFromMolecule();

//------------------------------------------------------------------------------
void ResultsFromMolecule::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
Molecule *ResultsFromMolecule::GetInput()
{
  return this->GetInput(0);
}

//------------------------------------------------------------------------------
Molecule *ResultsFromMolecule::GetInput(int port)
{
  return Molecule::SafeDownCast( this->GetInputDataObject(port, 0)) ;
}

//------------------------------------------------------------------------------
void ResultsFromMolecule::SetInput(Molecule *d)
{
  this->SetInputDataObject(0, d);
}
/*
//------------------------------------------------------------------------------
vtkDataObject *ResultsFromMolecule::GetOutput()
{
  return this->GetOutput(0);
}

//------------------------------------------------------------------------------
vtkDataObject *ResultsFromMolecule::GetOutput(int port)
{
  return this->GetExecutive()->GetOutputData(port, 0);
}
//------------------------------------------------------------------------------
Molecule *ResultsFromMolecule::GetMoleculeOutput(int port)
{
  return Molecule::SafeDownCast(this->GetInput(port));
}
*/

//------------------------------------------------------------------------------
vtkTypeBool ResultsFromMolecule::ProcessRequest(
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
int ResultsFromMolecule::FillOutputPortInformation(int vtkNotUsed(port), vtkInformation *info)
{
  // now add our info ???
  info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkMolecule"); // "vtkMolecule"???
  return 1;
}

//------------------------------------------------------------------------------
int ResultsFromMolecule::FillInputPortInformation(int vtkNotUsed(port), vtkInformation *info)
{
  // now add our info ???
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkMolecule"); // "vtkMolecule"???
  return 1;
}

//------------------------------------------------------------------------------
int ResultsFromMolecule::RequestInformation(vtkInformation *vtkNotUsed(request),
                                       vtkInformationVector **vtkNotUsed(inputVector),
                                       vtkInformationVector *vtkNotUsed(outputVector))
{
  // do nothing let subclasses handle it
  return 1;
}

//------------------------------------------------------------------------------
int ResultsFromMolecule::RequestUpdateExtent(vtkInformation *vtkNotUsed(request),
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
int ResultsFromMolecule::RequestData(vtkInformation *vtkNotUsed(request),
                                vtkInformationVector **vtkNotUsed(inputVector),
                                vtkInformationVector *vtkNotUsed(outputVector))
{
  return 1;
}
/*
//------------------------------------------------------------------------------
void ResultsFromMolecule::SetInputData(vtkDataObject *input)
{
  this->SetInputData(0, input);
}

//------------------------------------------------------------------------------
void ResultsFromMolecule::SetInputData(int index, vtkDataObject *input)
{
  this->SetInputDataInternal(index, input);
}

//------------------------------------------------------------------------------
void ResultsFromMolecule::AddInputData(vtkDataObject *input)
{
  this->AddInputData(0, input);
}

//------------------------------------------------------------------------------
void ResultsFromMolecule::AddInputData(int index, vtkDataObject *input)
{

  this->AddInputDataInternal(index, input);
}
*/