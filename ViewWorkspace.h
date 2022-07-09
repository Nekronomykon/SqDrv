#ifndef __Edit_Workspace_h__
#define __Edit_Workspace_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QString>
#include <QWidget>
#include <QPointer>
#include <QSplitter>
#include <QFileIconProvider>

#include "FrameDocument.h"

#include <QListWidget>
#include "ViewFileList.h"
#include "ModelWorkspace.h"

typedef QListWidget ListFileContent;

class ViewWorkspace
    : public QSplitter
{
  typedef QListWidget _Base;
  typedef FrameDocument::FileFormat FileFormat;

  Q_OBJECT
public:
  explicit ViewWorkspace(QWidget *parent = Q_NULLPTR);

  ViewFileList *getList() const { return listWorkPaths_; }

  void addFile(const QString & /* path */);

private:
  // QFileIconProvider icons_from_;
  QPointer<ViewFileList> listWorkPaths_;
  QPointer<ListFileContent> listContent_;
};

#endif // !__Edit_Workspace_h__