#include "ViewWorkspace.h"

#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QFileInfoList>
#include <QFileIconProvider>

#include <QListWidgetItem>

ViewWorkspace::ViewWorkspace(QWidget *parent)
    : QSplitter(Qt::Vertical, parent), listWorkPaths_(new ViewFileList), listContent_(new ListFileContent)
{
  // ModelWorkspace *pModel = new ModelWorkspace();
  // this->setModel(pModel);
  this->setOpaqueResize(false);
  this->insertWidget(0, listWorkPaths_);
  this->insertWidget(1, listContent_);
}

void ViewWorkspace::addFile(const QString &path)
{
  /*
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
      */
}
