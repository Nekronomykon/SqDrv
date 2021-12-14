#ifndef View_FileList_h__
#define View_FileList_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QListView>

class ViewFileList
    : public QListView
{
    Q_OBJECT
public:
    ViewFileList(QWidget * /*parent*/ = nullptr);
};

#endif // !View_FileList_h__
