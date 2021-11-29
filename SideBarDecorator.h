#ifndef __SideBar_Decorator_h__
#define __SideBar_Decorator_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QSize>
#include <QPaintEvent>
#include <QWidget>

class EditCode;

//![codeeditordefinition]
//![extraarea]

class SideBarDecorator
    : public QWidget
{
public:
  explicit SideBarDecorator(EditCode *editor);
  QSize sizeHint() const override;

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  EditCode *theHost_;
};

//![extraarea]

#endif // !__SideBar_Decorator_h__
