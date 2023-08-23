#ifndef TABDIALOG_H
#define TABDIALOG_H

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QDialog>
#include <QDialogButtonBox>
#include <QFileInfo>
#include <QTabWidget>

//! [0]
class GeneralTab : public QWidget
{
  Q_OBJECT

public:
  explicit GeneralTab(const QFileInfo &fileInfo, QWidget *parent = nullptr);
};
//! [0]

//! [1]
class PermissionsTab : public QWidget
{
  Q_OBJECT

public:
  explicit PermissionsTab(const QFileInfo &fileInfo, QWidget *parent = nullptr);
};
//! [1]

//! [2]
class ApplicationsTab : public QWidget
{
  Q_OBJECT

public:
  explicit ApplicationsTab(const QFileInfo &fileInfo, QWidget *parent = nullptr);
};
//! [2]

//! [3]
class DialogFileProperties : public QDialog
{
  Q_OBJECT

public:
  explicit DialogFileProperties(const QString &fileName, QWidget *parent = nullptr);
  explicit DialogFileProperties(const std::filesystem::path &fileName, QWidget *parent = nullptr);

protected:
  void BuildFileProperties(const QFileInfo &);

private:
  QTabWidget *tabWidget;
  QDialogButtonBox *buttonBox;
};
//! [3]

#endif // TABDIALOG_H
