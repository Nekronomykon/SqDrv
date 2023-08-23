#ifndef Model_Workspace_h__
#define Model_Workspace_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QStringListModel>

class ModelWorkspace
    : public QStringListModel
{
  Q_OBJECT
public:
  explicit ModelWorkspace();
};

#endif // !Model_Workspace_h__
