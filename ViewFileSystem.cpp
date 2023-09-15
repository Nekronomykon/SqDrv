#include "ViewFileSystem.h"

#include <QDir>

ViewFileSystem::ViewFileSystem(QWidget *parent)
    : QSplitter(Qt::Vertical, parent), model_(new ModelFiles),
      tree_(new ViewFilesTree(this)), list_(new ViewFilesList(this))
{
  model_->setIconProvider(&iconProvider_);
  model_->setOptions(ModelFiles::DontUseCustomDirectoryIcons);
  model_->setRootPath(QDir::rootPath());
  //
  tree_->setModel(model_);
  tree_->setCurrentIndex(model_->index(QDir::currentPath()));
  //
  list_->setModel(model_);
  list_->setRootIndex(model_->index(QDir::currentPath()));
}
