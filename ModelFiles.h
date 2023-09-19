#ifndef Model_Files_h__
#define Model_Files_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QFileSystemModel>

class ModelFiles : public QFileSystemModel
{
  Q_OBJECT
public:
  explicit ModelFiles();
  ~ModelFiles() override = default;
  private:
};

#endif // !Model_Files_h__
