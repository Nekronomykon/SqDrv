#ifndef Frame_Structure_h__
#define Frame_Structure_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FormatFile.h"

#include <QPointer>

#include "EditMarkLines.h"
#include "ViewMolecule.h"

#include <QTabWidget>

class FrameStructure : public QTabWidget
{
  Q_OBJECT

public:
  typedef TagFormatFile<FrameStructure> FileFormat;

  explicit FrameStructure(QWidget * /*parent*/ = nullptr);
  ~FrameStructure() override;

  bool isModified() const;
  void setModified(bool /*bSet*/ = true);
  //
  Path getPath(void) const;
  bool hasPath(void) const;
  Path resetPath(Path /*path*/ = Path());
  //
  ViewMolecule *getMoleculeView() const;
  MapMolecule *getMoleculeMap() const;
  Molecule *getMolecule() const;
  //
  void clearContent(void);
  void loadFile(void);
  bool exportFromPath(Path /*the_path*/ = Path());
  //
 
  static const FileFormat *AllFormats() { return &formatFile[0]; }

protected:
  static const FileFormat formatFile[];

private:
  bool bChanged_ = false;
  Path path_source_;
  NewMolecule molecule_;
  QPointer<ViewMolecule> viewMol_;
  QPointer<EditMarkLines> editSrc_;
};

#endif // !Frame_Structure_h__