#ifndef View_Files_h__
#define View_Files_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QFileSystemModel>

#include "ViewFileTree.h"
#include "ViewFileList.h"

#include <QPointer>

#include <QSplitter>

class ViewFiles
    : public QSplitter
{
    Q_OBJECT
public:
    ViewFiles(QWidget * /* parent */ = nullptr);
    ViewFileList *getList() const { return list_; }

private:
    QPointer<QFileSystemModel> model_;
    QPointer<ViewFileTree> tree_;
    QPointer<ViewFileList> list_;
};

#endif // !View_Files_h__
