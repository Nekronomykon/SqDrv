#ifndef Frame_Structure_h__
#define Frame_Structure_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FormatFile.h"
#include "ImplPathName.h"

#include <QPointer>

#include "ViewMolecule.h"
#include "EditSource.h"

#include <QTabWidget>

#include <map>

class FrameStructure : public QTabWidget, public ImplPathName<FrameStructure>
{
  Q_OBJECT

public:
  typedef TagFormatFile<FrameStructure> FileFormat;
  typedef std::map<String, FrameStructure::FileFormat> MolFormatMap;
  typedef std::multimap<String, FrameStructure::FileFormat> MolFormatMapEx;

  explicit FrameStructure(QWidget * /*parent*/ = nullptr);
  ~FrameStructure() override = default;

  template <class What>
  static String listFornatsToFilter(What if_it, bool bPreformat = true)
  {
    String sTypes;
    if (bPreformat)
      sTypes.assign("All known files (");
    String sAll("All files (*.*)");
    auto itFormat = suffixToFormat.cbegin();
    String sKnown;
    do
    {
      if (!if_it(itFormat->second))
        continue;
      // by format
      String sFormat(itFormat->second.getName());
      sFormat += " files (";

      String sMask("*");
      sMask += itFormat->first; // <-- *.ext

      sFormat += sMask;
      sFormat += ");;";

      // all known
      sKnown += sFormat;
      if (!sTypes.empty())
      {
        sTypes += sMask;
        sTypes += " ";
      }
  } while (++itFormat != suffixToFormat.cend());

  if (!sTypes.empty())
  {
      // *sTypes.rbegin() = 0;
      sTypes += ");;"; // all-known mask is closed
  }
  sTypes += sKnown;
  sTypes += sAll;

  return sTypes;
  }
  //
  static QStringList listAllExtensions();
  //
  template <class What>
  static QStringList listExtensionsFor(What it_is)
  {
    QStringList res;
    auto itFormatTag = suffixToFormat.cbegin();
    do
    {
      QString extz("*");
      if (!itFormatTag->first.empty() && it_is(itFormatTag->second))
      {
        extz += itFormatTag->first.c_str();
        res << extz;
      }
      /* code */
    } while (++itFormatTag != suffixToFormat.cend());

    return res;
  }

  bool isModified() const;
  void setModified(bool /*bSet*/ = true);
  //
  //
  EditSource *editSource();
  EditSource *getEditSource() const;
  ViewMolecule *viewMolecule();
  ViewMolecule *getViewMolecule() const;
  MapMolecule *getMoleculeMap() const;
  Molecule *getMolecule() const;
  void resetTitle(String /*title*/ = String());
  //
  void clearContent(void);
  void loadPath(Path /* a_path */);
  void reloadCurrentFile(void);
  bool importFromPath(Path /* a_path */);
  bool exportToPath(Path /* a_path */);
  //

  static size_t countFormatsByExt(String sext);

protected:
  static const MolFormatMap suffixToFormat; // convert to multimap?

private:
  bool bChanged_ = false;
  NewMolecule molecule_;
  QPointer<ViewMolecule> viewMol_;
  QPointer<EditSource> editSrc_;
};

#endif // !Frame_Structure_h__
