#ifndef View_FileSystem_h__
#define View_FileSystem_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QSplitter>

#include <QFileIconProvider>
#include <QPointer>

#include "ModelFiles.h"
#include "ViewFilesTree.h"
#include "ViewFilesList.h"

class ViewFileSystem
    : public QSplitter
{
  Q_OBJECT
public:
  explicit ViewFileSystem(QWidget * /*parent*/ = nullptr);
  ~ViewFileSystem() override = default;

private:
  QPointer<ModelFiles> model_;
  QPointer<ViewFilesTree> tree_;
  QPointer<ViewFilesList> list_;
  QFileIconProvider iconProvider_;
};

#endif // !View_FileSystem_h__