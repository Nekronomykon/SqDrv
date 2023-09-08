#include "EditSource.h"

#include "Elements.h"
#include "Formula.h"
using namespace vtk;

#include "ImplPathName.h"

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
QString EditSource::getTitleString(void) const
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
EditMarkLines *EditSource::getEditAtoms(void) const
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
void EditSource::showMolecule(void)
{
  editAtoms_->clear();
  editNumberOfAtoms_->clear();
  editFormula_->clear();

  vtkIdType nAtoms = !ptrMolecule_
                         ? 0
                         : ptrMolecule_->GetNumberOfAtoms();

  editNumberOfAtoms_->setText(QString::number(nAtoms));
  Formula form(*ptrMolecule_);
  editFormula_->setText(QString(form.asString().c_str()));

  if (ptrMolecule_ && (nAtoms > 0)о)
  {
    vtkIdType idAtom = 0;
    vtkIdType idFragment = -1;
    QTextCursor cursor(editAtoms_->document());
    do
    {
      vtkAtom atomHere = ptrMolecule_->GetAtom(idAtom);

      /* Atom --> String */
      OutputString outs;
      outs << std::setw(4) << Elements::GetElementSymbol(atomHere.GetAtomicNumber());

      /* code */
      if (idFragment > 0)
      {
        // strAtom += tr("(%1)").arg(idFragment);
      }
      outs
          << std::fixed
          << std::setw(14) << std::setprecision(8) << atomHere.GetMolecule()->GetPoint(atomHere.GetId())[0]
          << std::setw(14) << std::setprecision(8) << atomHere.GetMolecule()->GetPoint(atomHere.GetId())[1]
          << std::setw(14) << std::setprecision(8) << atomHere.GetMolecule()->GetPoint(atomHere.GetId())[2]
          << std::endl;
      ++idAtom; // next atom:

      // cursor.insertText(strAtom);
      cursor.insertText(outs.str().c_str());
      // cursor.insertBlock();
    } while (idAtom < nAtoms);
  }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void EditSource::readMolecule(void)
{
  NewMolecule mol_read;
  auto* pDoc = editAtoms_->document();
  QTextBlock blkText = pDoc->begin();
  vtkIdType idAtom = mol_read->GetNumberOfAtoms();
  do
  {
    if(blkText.isValid())
    {
      InputString inps(String( blkText.text().toLatin1().data() ));
    }
    /* code */
    blkText = blkText.next();
  } while (blkText == pDoc->end());
  
  }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
