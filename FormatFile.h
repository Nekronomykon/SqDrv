#ifndef Tag_File_Format_h__
#define Tag_File_Format_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <string>
using String = std::string;

#include <utility>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;
using Path = fs::path;
// using Path = String;

template <class Host>
class TagFormatFile
{
public:
  typedef bool (*DataParser)(Path, Host &);
  typedef bool (*DataWriter)(Host &, Path);

  explicit TagFormatFile(String nameFmt,
                         DataParser read = nullptr,
                         DataWriter save = nullptr)
      : nameFormat_(nameFmt), operationRead_(read), operationSave_(save)
  {
  }
  //
  String getName() const { return nameFormat_; }
  bool hasName() const { return !nameFormat_.empty(); }
  bool hasRead() const { return bool(operationRead_ != nullptr); }
  bool hasSave() const { return bool(operationSave_ != nullptr); }
  //
  bool isValid() const { return (hasRead() || hasSave()) && hasName(); }
  bool isNative() const { return hasRead() && hasSave() && hasName(); }
  bool isToLoad() const { return hasRead() && hasName(); }
  bool isToSave() const { return hasSave() && hasName(); }
  bool applyReadTo(Host &host, Path the_path) const
  {
    return (!this->hasRead())
               ? false
               : (*operationRead_)(the_path, host);
  }
  bool applySaveTo(Host &host, Path the_path) const
  {
    return (!this->hasSave())
               ? false
               : (*operationSave_)(host, the_path);
  }
  //
private:
  String nameFormat_ = String();
  DataParser operationRead_ = nullptr;
  DataWriter operationSave_ = nullptr;
};

struct IsFormatValid
{
  template <class Host>
  bool operator()(const TagFormatFile<Host> &tag) const
  {
    return tag.isValid();
  }
};

struct IsFormatToLoad
{
  template <class Host>
  bool operator()(const TagFormatFile<Host> &tag) const
  {
    return tag.isToLoad();
  }
};

struct IsFormatToSave
{
  template <class Host>
  bool operator()(const TagFormatFile<Host> &tag) const
  {
    return tag.isToSave();
  }
};

struct IsFormatNative // both save and load
{
  template <class Host>
  bool operator()(const TagFormatFile<Host> &tag) const
  {
    return tag.isNative();
  }
};

//

#endif // !Tag_File_Format_h__
