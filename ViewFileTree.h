#ifndef View_FileTree_h__
#define View_FileTree_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QTreeView>

class ViewFileTree
    : public QTreeView
{
    Q_OBJECT
public:
    ViewFileTree(QWidget * /*parent*/ = nullptr);
};

#endif // !View_FileTree_h__
