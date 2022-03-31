#ifndef Model_Workspace_h
#define Model_Workspace_h

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FrameDocument.h"

#include <QStringList>
#include <QAbstractListModel>
#include <QVariant>

#include <QVector>
#include <QPair>
#include <QMap>

class ModelWorkspace
    : public QAbstractListModel
{
  Q_OBJECT
public:
/*
  typedef FrameDocument Child;
  typedef Child::FileFormat  FileFormat;
  typedef FileFormat::NameFormat NameFile;
  */
  // typedef QVector<QPair<NameFormat, FileFormat> > Workspace;
  typedef QStringList Workspace;

  explicit ModelWorkspace();
  ~ModelWorkspace() override;

  int columnCount(const QModelIndex &/*parent*/ = QModelIndex()) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

private:
  Workspace workspace_;
};

#endif // !Model_Workspace_h
