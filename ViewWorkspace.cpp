#include "ViewWorkspace.h"

#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QFileInfoList>
#include <QFileIconProvider>

#include <QListWidgetItem>

ViewWorkspace::ViewWorkspace(QWidget *parent)
  : _Base(parent)
{
  this->setViewMode(QListView::ListMode);
  //ModelWorkspace *pModel = new ModelWorkspace();
  //this->setModel(pModel);
}

bool ViewWorkspace::addFilePath(const QString &path, FileFormat fmt)
{
  QListWidgetItem *pFileItem = nullptr;
  QFileInfo finfo(path);

  if (!finfo.isFile())
    return false;

  bool result(false);
  QString s_query = finfo.canonicalFilePath();

  QList<QListWidgetItem *> added = this->findItems(s_query, Qt::MatchExactly);

  if (added.isEmpty())
    {
      QFileIconProvider icons;
      QIcon ic(icons.icon(finfo));
      pFileItem = new QListWidgetItem(ic, s_query, this);
      // pFileItem->setData(Qt::UserRole, fmt));
      result = true;
    }
  else
    {
      pFileItem = added.front(); // and the only item, I hope...
    }

  return result;
}

void ViewWorkspace::listDir(const QString &dir_path, bool bRecurse)
{
  QDir dir_entry(dir_path);
  QDir::Filters flt_use = QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable;

  QFileInfoList fi_list = dir_entry.entryInfoList(flt_use, QDir::Name);
  foreach (QFileInfo fi, fi_list)
    {
      this->addFilePath(fi.canonicalFilePath());
    }
  if (bRecurse)
    {
      QDir::Filters flt_dirs = QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks;
      fi_list = dir_entry.entryInfoList(flt_dirs, QDir::Name);
      foreach (QFileInfo fi, fi_list)
        {
          this->listDir(fi.canonicalFilePath(), true);
        }
    }
}
