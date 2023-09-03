#ifndef Edit_Source_h__
#define Edit_Source_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QPointer>
#include <QLineEdit>

#include <memory>

#include "EditMarkLines.h"

#include "ui_EditSource.h"

class EditSource
    : public QWidget,
      protected Ui_EditSource
{
  Q_OBJECT
public:
  explicit EditSource(QWidget * /*parent*/ = nullptr);
  bool isModified(void) const;
  //
  QString getTitleString(void)const;
  // QString getTitle(void);
  void resetTitleString(QString /*strTitle*/ = QString());
  //
  EditMarkLines *getEditAtoms() const;

private:
};

#endif // !Edit_Source_h__
