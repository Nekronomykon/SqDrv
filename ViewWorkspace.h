#ifndef View_Workspace_h__
#define View_Workspace_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QPointer>

#include "ModelWorkspace.h"

#include <QListView>

class ViewWorkspace
    : public QListView
{
  Q_OBJECT
public:
  explicit ViewWorkspace(QWidget * /*parent*/ = nullptr);

    template<class Iter>
  void includePathsRange(Iter from, Iter to);

private:
  QPointer<ModelWorkspace> model_;
};

#endif // !View_Workspace_h__
