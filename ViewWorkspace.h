#ifndef View_Workspace_h__
#define View_Workspace_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QStringListModel>

#include <QListWidget>

class ModelFileList
    : public QStringListModel
{
};

typedef QListWidget ViewWorkspace;

#endif //!View_Workspace_h__