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
  ~FrameStructure() override;

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
  void loadFile(void);
  bool importFromPath(Path /*the_path*/ = Path());
  bool exportToPath(Path /*the_path*/);
  //

  static const FileFormat *AllFormats() { return &formatFile[0]; }

protected:
  static const FileFormat formatFile[]; // equivalent to map, convert to multimap?
  static const MolFormatMap suffixToFormat;

private:
  bool bChanged_ = false;
  NewMolecule molecule_;
  QPointer<ViewMolecule> viewMol_;
  QPointer<EditSource> editSrc_;
};

#endif // !Frame_Structure_h__
