#ifndef Results_From_Molecule_h__
#define Results_From_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "Molecule.h"

#include <vtkAlgorithm.h>

class ResultsFromMolecule
    : public vtkAlgorithm
{
public:
  static ResultsFromMolecule *New();
  vtkTypeMacro(ResultsFromMolecule, vtkAlgorithm);
  void PrintSelf(ostream &os, vtkIndent indent) override;

  ///@{
  /**
   * Get the output data object for a port on this algorithm.
   */
  Molecule *GetInput();
  Molecule *GetInput(int);
  virtual void SetInput(Molecule *d);
  ///@}

  /**
   * see vtkAlgorithm for details
   */
  vtkTypeBool ProcessRequest(vtkInformation *,
                             vtkInformationVector **,
                             vtkInformationVector *) override;

  // this method is not recommended for use, but lots of old style filters
  // use it
  // vtkDataObject *GetOutput();
  // vtkDataObject *GetOutput(int port);
  // Molecule *GetMoleculeOutput(int port);

  ///@{
  /**
   * Set an input of this algorithm. You should not override these
   * methods because they are not the only way to connect a pipeline.
   * Note that these methods support old-style pipeline connections.
   * When writing new code you should use the more general
   * vtkAlgorithm::SetInputConnection().  These methods transform the
   * input index to the input port index, not an index of a connection
   * within a single port.
   */
  void SetOutputData(vtkDataObject *);
  void SetOutputData(int, vtkDataObject *);
  ///@}

  ///@{
  /**
   * Add an input of this algorithm.  Note that these methods support
   * old-style pipeline connections.  When writing new code you should
   * use the more general vtkAlgorithm::AddInputConnection().  See
   * SetInputData() for details.
   */
  void AddOutputData(vtkDataObject *);
  void AddOutputData(int, vtkDataObject *);
  ///@}

protected:
  explicit ResultsFromMolecule(int /*nOuts */ = 1, int /* nIns */ = 1);
  ~ResultsFromMolecule() override = default;

  // convenience method
  virtual int RequestInformation(vtkInformation *request, vtkInformationVector **inputVector,
                                 vtkInformationVector *outputVector);

  /**
   * This is called by the superclass.
   * This is the method you should override.
   */
  virtual int RequestData(vtkInformation *request, vtkInformationVector **inputVector,
                          vtkInformationVector *outputVector);

  /**
   * This is called by the superclass.
   * This is the method you should override.
   */
  virtual int RequestUpdateExtent(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  // see algorithm for more info
  int FillOutputPortInformation(int port, vtkInformation *info) override;
  int FillInputPortInformation(int port, vtkInformation *info) override;

private:
  ResultsFromMolecule(const ResultsFromMolecule &) = delete;
  void operator=(const ResultsFromMolecule &) = delete;
};

#endif // !Results_From_Molecule_h__
