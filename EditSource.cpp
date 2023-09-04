#include "EditSource.h"

#include "Elements.h"
#include "Formula.h"
using namespace vtk;

#include <QTextCursor>
#include <QTextBlock>
#include <QStringLiteral>
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
EditSource::EditSource(QWidget *parent)
    : QWidget(parent)
{
  this->setupUi(this);
  // update edit fonts:
  QFont fontAtoms(editAtoms_->font());
  this->updateFont(fontAtoms);
}
void EditSource::updateFont(QFont f)
{
  editAtoms_->setFont(f);
  editTitle_->setFont(f);
  editFormula_->setFont(f);
  editNumberOfAtoms_->setFont(f);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool EditSource::isModified(void) const
{
  return editTitle_->isModified() || editAtoms_->isModified();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
QString EditSource::getTitleString() const
{
  return editTitle_->text();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void EditSource::resetTitleString(QString str)
{
  editTitle_->setText(str);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
EditMarkLines *EditSource::getEditAtoms() const
{
  return editAtoms_;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
Molecule *EditSource::resetMolecule(Molecule *pMol)
{
  if (pMol != ptrMolecule_)
    std::swap(ptrMolecule_, pMol);
  return pMol;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void EditSource::showMolecule()
{
  editAtoms_->clear();
  editNumberOfAtoms_->clear();
  editFormula_->clear();

  vtkIdType nAtoms = !ptrMolecule_
                         ? 0
                         : ptrMolecule_->GetNumberOfAtoms();

  editNumberOfAtoms_->setText(QString::number(nAtoms));
  Formula form(*ptrMolecule_);
  editFormula_->setText(QString( form.asString().c_str() ));

  if (ptrMolecule_ && (nAtoms > 0))
  {
    vtkIdType idAtom = 0;
    vtkIdType idFragment = -1;
    QTextCursor cursor(editAtoms_->document());
    do
    {
      vtkAtom atomHere = ptrMolecule_->GetAtom(idAtom);
      QString strAtom(tr(" %1").arg(Elements::GetElementSymbol(atomHere.GetAtomicNumber()).c_str(), 4));

      /* code */
      if (idFragment > 0)
      {
        strAtom += tr("(%1)").arg(idFragment);
      }
      strAtom += "  ";
      strAtom += (tr("  %1  %2  %3")
                      .arg(ptrMolecule_->GetPoint(idAtom)[0], 13, 'f', 8)
                      .arg(ptrMolecule_->GetPoint(idAtom)[1], 13, 'f', 8)
                      .arg(ptrMolecule_->GetPoint(idAtom)[2], 13, 'f', 8));
      ++idAtom; // next atom:

      cursor.insertText(strAtom);
      cursor.insertBlock();
    } while (idAtom < nAtoms);
  }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
