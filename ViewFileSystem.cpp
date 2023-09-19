#include "ViewFileSystem.h"

#include <QDir>
#include <QMessageBox>

ViewFileSystem::ViewFileSystem(QWidget *parent)
    : QSplitter(Qt::Vertical, parent), model_(new ModelFiles),
      tree_(new ViewFilesTree(this)), list_(new ViewFilesList(this))
{
  model_->setIconProvider(&iconProvider_);
  model_->setOptions(ModelFiles::DontUseCustomDirectoryIcons);
  //
  model_->setRootPath(QDir::rootPath());
  //
  tree_->setAllColumnsShowFocus(true);
  tree_->setModel(model_);
  // only tree to see
  tree_->setColumnHidden(1, true);
  tree_->setColumnHidden(2, true);
  tree_->setColumnHidden(3, true);
  // set it up
  tree_->setCurrentIndex(model_->index(QDir::currentPath()));
  //
  list_->setModel(model_);
  list_->setRootIndex(model_->index(QDir::currentPath()));
  //
  // list_->setSelectionModel(tree_->selectionModel());
  //
  connect(tree_, &QTreeView::activated, this, &ViewFileSystem::listActiveDir);
  connect(list_, &QListView::activated, this, &ViewFileSystem::dirFromList);
}

void ViewFileSystem::listActiveDir(const QModelIndex &idx)
{
  if (!idx.isValid())
    return;

  strFilePath_ = idx.data().toString();
  if (model_->isDir(idx))
  {
    QDir::setCurrent(idx.data().toString());
    list_->setRootIndex(idx);
  }
  else
  {
    QDir::setCurrent(idx.parent().data().toString());
    list_->setRootIndex(idx.parent());
    list_->scrollTo(idx);
    list_->setCurrentIndex(idx);
  }
}

void ViewFileSystem::dirFromList(const QModelIndex &idx)
{
  if (!idx.isValid())
    return;

  strFilePath_ = idx.data().toString();
  tree_->scrollTo(idx);
  tree_->setCurrentIndex(idx);
  if (model_->isDir(idx))
  {
    list_->setRootIndex(idx);
  }
  else
  {
    list_->setRootIndex(idx.parent());
  }
}

// void ViewFileSystem::showFilePath(const Path &a_path)
// {
//   this->ShowFilePath(QString(a_path.c_str()));
// }
void ViewFileSystem::showFilePath(const QString &q_path)
{
  QModelIndex idx = model_->index(q_path);
  if (idx.isValid())
  {
    tree_->scrollTo(idx);
    tree_->setCurrentIndex(idx);
    list_->setRootIndex(idx.parent());
    list_->setCurrentIndex(idx);
    strFilePath_ = q_path;
  }
}

ModelFiles *ViewFileSystem::getModelFiles() const
{
  return model_;
}

ViewFilesTree *ViewFileSystem::getFileTree() const
{
  return tree_;
}

ViewFilesList *ViewFileSystem::getFileList() const
{
  return list_;
}
