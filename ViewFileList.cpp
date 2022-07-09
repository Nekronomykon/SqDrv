#include "ViewFileList.h"

#include <QListWidgetItem>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QDir>

ViewFileList::ViewFileList(QWidget *parent)
    : QListView(parent)
{
    this->setViewMode(QListView::ListMode);
}
/*
void ViewFileList::listDir(const QString &dir_path, bool bRecurse)
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
*/
