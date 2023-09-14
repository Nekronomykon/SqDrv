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

  explicit TagFormatFile(String maskFmt,
                         String nameFmt,
                         DataParser read = nullptr,
                         DataWriter save = nullptr)
      : nameFormat_(nameFmt), maskFormat_(maskFmt), operationRead_(read), operationSave_(save)
  {
  }
  explicit TagFormatFile(String nameFmt,
                         DataParser read = nullptr,
                         DataWriter save = nullptr)
      : nameFormat_(nameFmt), maskFormat_(), operationRead_(read), operationSave_(save)
  {
  }
  //
  String getName() const { return nameFormat_; }
  String getMask() const { return maskFormat_; }
  bool conforms(Path a_path) const
  {
    return !hasMask()
               ? false
               : bool(!maskFormat_.compare(a_path.extension().string()));
  }
  //
  bool hasName() const { return !nameFormat_.empty(); }
  bool hasMask() const { return !maskFormat_.empty(); }
  bool hasRead() const { return bool(operationRead_ != nullptr); }
  bool hasSave() const { return bool(operationSave_ != nullptr); }
  //
  bool isValid() const { return (hasRead() || hasSave()) && hasName(); }
  bool isNative() const { return hasRead() && hasSave() && hasName() ; }
  bool isToLoad() const { return hasRead() && hasName(); }
  bool isToSave() const { return hasSave() && hasName(); }
  //
  bool applyReadTo(Host &host, Path the_path) const
  {
    return (!hasRead() || !conforms(the_path))
               ? false
               : (*operationRead_)(the_path, host);
  }
  //
  bool applySaveTo(Host &host, Path the_path) const
  {
    return (!hasSave() || !conforms(the_path))
               ? false
               : (*operationSave_)(host, the_path);
  }
  //
private:
  String maskFormat_ = String(".*");
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

struct IsFormatNative
{
  template <class Host>
  bool operator()(const TagFormatFile<Host> &tag) const
  {
    return tag.isNative();
  }
};

//
template <class Host, class What>
String BuildFileDialogFilter(const Host &host, What if_it, bool bPreformat = true)
{
  String sAll("All files (*.*)");
  const auto *ptrFormat = Host::AllFormats();
  if (!ptrFormat)
    return sAll; // not any formats registered
  String sTypes;
  if (bPreformat)
    sTypes.assign("All known files (");
  String sKnown;
  do
  {
    if (!if_it(*ptrFormat))
      continue;
    // by format
    String sFormat(ptrFormat->getName().c_str());
    sFormat += " files (";

    String sMask("*");
    sMask += ptrFormat->getMask(); // <-- *.ext

    sFormat += sMask;
    sFormat += ");;";

    // all known
    sKnown += sFormat;
    if (!sTypes.empty())
    {
      sTypes += sMask;
      sTypes += " ";
    }
  } while ((++ptrFormat)->isValid());

  if (!sTypes.empty())
  {
    // *sTypes.rbegin() = 0;
    sTypes += ");;"; // all-known mask is closed
  }
  sTypes += sKnown;
  sTypes += sAll;

  return sTypes;
}

#endif // !Tag_File_Format_h__
