#include "EditSource.h"

#include <QFile>
#include <QFileInfo>

#include "ResetCursor.h"

/* inline static */ QString EditSource::nameTemplate()
{
  return tr(name_template);
}

QString EditSource::dumpTemporary()
{
  QString path_res;
  // save the current content to a temporary file
  QPointer<QTemporaryFile>
    tmp_dump = new QTemporaryFile(nameTemplate());

  ResetCursor rc(QCursor(Qt::WaitCursor));

  bool bRO = this->isReadOnly();
  this->setReadOnly(true);

  if (tmp_dump->open())
    this->save(tmp_dump);

  if (tmp_dump->flush())
    path_res = tmp_dump->fileName();

  tmp_dump->close();
  this->setReadOnly(bRO);

  QFileInfo fi(path_res);
  return fi.canonicalFilePath();
}
