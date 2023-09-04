#ifndef Formula_h__
#define Formula_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "Molecule.h"

#include "Elements.h"

#include <map>

namespace vtk
{
  class Formula
  {
  public:
    explicit Formula(/*const*/ Molecule &mol);
    explicit Formula() {}

    String asString() const;

  protected:
    typedef std::map<IndexElement, unsigned long, ElementPreceeds> FormulaMap;

    template <class Mol>
    void buildMap(/*const*/ Mol &mol)
    {
      FormulaMap map;
      int nAtoms = mol.GetNumberOfAtoms();
      for (int i = 0; i < nAtoms;
           ++map[mol.GetAtomAtomicNumber(i++)])
        ;
      formula_ = FormulaMap(map);
    }

  private:
    FormulaMap formula_;
  };

}; // !namespace vtk

#endif // !Formula_h__
