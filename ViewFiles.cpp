#include "ViewFiles.h"

#include <QDir>
#include <QModelIndex>
#include <QFileSystemModel>

ViewFiles::ViewFiles(QWidget *parent)
    : QSplitter(Qt::Vertical, parent), model_(new QFileSystemModel), tree_(new ViewFileTree(this)), list_(new ViewFileList(this))
{
    model_->setOptions(QFileSystemModel::DontUseCustomDirectoryIcons);
    model_->setRootPath(QDir::rootPath());
    //   
    QModelIndex idx = model_->index(QDir::currentPath());
    //
    tree_->setModel(model_);
    tree_->setCurrentIndex(idx);
    list_->setModel(model_);
    list_->setCurrentIndex(idx);
    //
    this->setOpaqueResize(false);
    this->insertWidget(0, tree_);
    this->insertWidget(1, list_);
}