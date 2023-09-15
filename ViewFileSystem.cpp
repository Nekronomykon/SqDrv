#include "ViewFileSystem.h"

#include "FrameStructure.h"

#include <QDir>
#include <QMessageBox>

ViewFileSystem::ViewFileSystem(QWidget *parent)
    : QSplitter(Qt::Vertical, parent), model_(new ModelFiles),
      tree_(new ViewFilesTree(this)), list_(new ViewFilesList(this))
{
  model_->setNameFilterDisables(true); // false?
  model_->setNameFilters(FrameStructure::listExtensionsFor( IsFormatToLoad() ));
  //
  model_->setIconProvider(&iconProvider_);
  model_->setOptions(ModelFiles::DontUseCustomDirectoryIcons);
  //
  model_->setRootPath(QDir::rootPath());
  //
  tree_->setAllColumnsShowFocus(true);
  tree_->setModel(model_);
  tree_->setCurrentIndex(model_->index(QDir::currentPath()));
  //
  list_->setModel(model_);
  list_->setRootIndex(model_->index(QDir::currentPath()));
  //
  connect(tree_, &QTreeView::activated, this, &ViewFileSystem::listActiveDir);
}

void ViewFileSystem::listActiveDir(const QModelIndex &idx)
{
  if (!idx.isValid())
    return;
  if (model_->isDir(idx))
    list_->setRootIndex(idx);
  else
    list_->setRootIndex(idx.parent());
  // QMessageBox::information(list_,model_->fileName(idx),model_->filePath(idx));
  /// list_->setRootPath()
}