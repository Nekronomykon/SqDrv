#ifndef __Edit_Workspace_h__
#define __Edit_Workspace_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QString>
#include <QWidget>
#include <QFileIconProvider>

#include "FrameDocument.h"

#include <QListWidget>
#include "ModelWorkspace.h"

class ViewWorkspace
  : public QListWidget
{
  typedef QListWidget _Base;
  typedef FrameDocument::FileFormat FileFormat;

  Q_OBJECT
public:
  explicit ViewWorkspace(QWidget *parent = Q_NULLPTR);

  bool addFilePath(const QString & /* path */, FileFormat fmt = FileFormat());
  void listDir(const QString& /*dir_path*/, bool /*bRecurse*/ = false);

private:
  // QFileIconProvider icons_from_;
};

#endif // !__Edit_Workspace_h__