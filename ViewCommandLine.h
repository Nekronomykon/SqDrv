#ifndef View_CommandLine_h__
#define View_CommandLine_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "ui_ViewCommandLine.h"

#include <QWidget>

class ViewCommandLine
    : public QWidget,
    private Ui_ViewCommandLine
{
    Q_OBJECT
private:
    /* data */
public:
    ViewCommandLine(QWidget * /*parent*/ = nullptr);
    ~ViewCommandLine();
};

#endif // !Frame_CommandLine_h__
