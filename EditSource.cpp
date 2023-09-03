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

QString EditSource::getTitleString() const
{
  return editTitle_->text();
}

void EditSource::resetTitleString(QString str)
{
  editTitle_->setText(str);
}

EditMarkLines *EditSource::getEditAtoms() const
{
  return editAtoms_;
}
