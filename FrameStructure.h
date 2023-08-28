#ifndef Frame_Structure_h__
#define Frame_Structure_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FormatFile.h"
#include "ImplPathName.h"

#include <QPointer>

#include "EditMarkLines.h"
#include "ViewMolecule.h"

#include <QTabWidget>

class FrameStructure : public QTabWidget
, public ImplPathName<FrameStructure>
{
  Q_OBJECT

public:
  typedef TagFormatFile<FrameStructure> FileFormat;

  explicit FrameStructure(QWidget * /*parent*/ = nullptr);
  ~FrameStructure() override;

  bool isModified() const;
  void setModified(bool /*bSet*/ = true);
  //
  //
  ViewMolecule *getMoleculeView() const;
  MapMolecule *getMoleculeMap() const;
  Molecule *getMolecule() const;
  //
  void clearContent(void);
  void loadFile(void);
  bool importFromPath(Path /*the_path*/ = Path());
  bool exportToPath(Path /*the_path*/);
  //
 
  static const FileFormat *AllFormats() { return &formatFile[0]; }

protected:
  static const FileFormat formatFile[];

private:
  bool bChanged_ = false;
  NewMolecule molecule_;
  QPointer<ViewMolecule> viewMol_;
  QPointer<EditMarkLines> editSrc_;
};

#endif // !Frame_Structure_h__
