#include "EditSource.h"

EditSource::EditSource(QWidget *parent)
    : QWidget(parent)
{
  this->setupUi(this);
}

bool EditSource::isModified(void) const
{
  return editTitle_->isModified() || editAtoms_->isModified();
}

EditMarkLines *EditSource::getEditAtoms() const
{
  return editAtoms_;
}
