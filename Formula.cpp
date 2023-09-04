#include "Formula.h"
#include "ImplPathName.h"
using namespace vtk;

Formula::Formula(/*const*/ Molecule &mol)
{
  this->buildMap(mol);
}

String Formula::asString() const
{
  // String result;
  OutputString outs;
  if (!formula_.empty())
  {
    auto itBegin = formula_.cbegin();
    do
    {
      if (!Elements::IsValidAtomNumber(itBegin->first))
        continue;
      String sElement = Elements::GetElementSymbol(itBegin->first);
      if (itBegin->second)
      {
        outs << sElement << ' ';
        if (itBegin->second > 1)
          outs << itBegin->second << ' ';
      }

    } while (++itBegin != formula_.cend());
  }
  return String(outs.str());
}
