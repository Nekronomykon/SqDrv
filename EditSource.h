#ifndef EditSource_h__
#define EditSource_h__

#ifdef _MSC_VER
#  pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QFileInfo>
#include <QMessageBox>
#include <QPointer>
#include <QString>
#include <QTemporaryFile>

#include "EditCode.h"
#include "ResetCursor.h"

class EditSource : public EditCode
{
  Q_OBJECT
public:
  explicit EditSource(QWidget* parent = nullptr)
    : EditCode(parent)
  {
  }
  ~EditSource() override = default;

  // save the current content to a temporary file and return its path
  QString dumpTemporary();

  template <typename Parser>
  bool parseCurrentContentWith(Parser* parser)
  {
    // save the current content to a temporary file
    // QString path_res = this->dumpTemporary();
    ResetCursor rc(QCursor(Qt::WaitCursor));
    QTemporaryFile file_tmp(EditSource::nameTemplate());
    if (!file_tmp.open())
      return false;

    QString path_res = file_tmp.fileName();
    QFileInfo fi(path_res);
    path_res = fi.canonicalFilePath();
    bool bSysOk = !path_res.isEmpty();

    if (!bSysOk)
      return false;

    this->save(&file_tmp);
    file_tmp.flush();
    file_tmp.close();
    QByteArray bytes = path_res.toLatin1();
    parser->SetFileName(bytes.data());
    parser->Update();
#ifndef QT_MESSAGE_BOX_INFO_DEBUG
    QString message(
      tr("Reading molecule from path %1:\nNumber of atoms read: %2"));
    QMessageBox::information(
      this, QString("parseCurrentContentWith()"),
      message.arg(bytes.data()).arg(parser->GetOutput()->GetNumberOfAtoms()));
#endif //! QT_MESSAGE_BOX_INFO_DEBUG

    return true;
  }

  static QString nameTemplate();

private:
  static constexpr const char* name_template = "ScD-XXXXXX-src.sqdrv";
};

#endif //! EditSource_h__
