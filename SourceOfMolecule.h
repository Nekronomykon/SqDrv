#ifndef Molecule_Source_h__
#define Molecule_Source_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "Molecule.h"

#include <vtkAlgorithm.h>

class MoleculeSource
    : public vtkAlgorithm
{
public:
  static MoleculeSource *New();
  vtkTypeMacro(MoleculeSource, vtkAlgorithm);
  void PrintSelf(ostream &os, vtkIndent indent) override;

  ///@{
  /**
   * Get the output data object for a port on this algorithm.
   */
  Molecule *GetOutput();
  Molecule *GetOutput(int);
  virtual void SetOutput(Molecule *d);
  ///@}

  /**
   * see vtkAlgorithm for details
   */
  vtkTypeBool ProcessRequest(vtkInformation *,
                             vtkInformationVector **,
                             vtkInformationVector *) override;

  // this method is not recommended for use, but lots of old style filters
  // use it
  vtkDataObject *GetInput();
  vtkDataObject *GetInput(int port);
  Molecule *GetMoleculeInput(int port);

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
  void SetInputData(vtkDataObject *);
  void SetInputData(int, vtkDataObject *);
  ///@}

  ///@{
  /**
   * Add an input of this algorithm.  Note that these methods support
   * old-style pipeline connections.  When writing new code you should
   * use the more general vtkAlgorithm::AddInputConnection().  See
   * SetInputData() for details.
   */
  void AddInputData(vtkDataObject *);
  void AddInputData(int, vtkDataObject *);
  ///@}

protected:
  explicit MoleculeSource(int /*nOuts */ = 1);
  ~MoleculeSource() override = default;

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
  MoleculeSource(const MoleculeSource &) = delete;
  void operator=(const MoleculeSource &) = delete;
};

#endif // !Molecule_Source_h__
