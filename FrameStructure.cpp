#include "FrameStructure.h"

#include <vtkMoleculeReaderBase.h>
#include <vtkMoleculeAlgorithm.h>

#include "AcquireFileCML.h"
#include "AcquireFileCUBE.h"
#include "AcquireFilePDB.h"
#include "AcquireFileXYZ.h"

#include "ExportFilePNG.h"

#include <QMessageBox>

bool ReadDataFormatCML(Path a_path, FrameStructure &host) { return ParseFileCMLTo(a_path, host); }
bool ReadDataFormatPDB(Path a_path, FrameStructure &host) { return ParseFilePDBTo(a_path, host); }
bool ReadDataFormatXYZ(Path a_path, FrameStructure &host) { return ParseFileXYZTo(a_path, host); }
bool ReadDataFormatCUBE(Path a_path, FrameStructure &host) { return ParseFileCUBETo(a_path, host); }
//
bool WriteImageFormatPNG(FrameStructure &host, Path a_path) { return ExportToPNGFile(host, a_path); }
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
//
const FrameStructure::FileFormat FrameStructure::formatFile[] = {
    FileFormat("Chemical Markup Language", ".cml", ReadDataFormatCML),
    FileFormat("XMol atomic data", ".xyz", ReadDataFormatXYZ),
    FileFormat("Brookhaven data bank", ".pdb", ReadDataFormatPDB),
    FileFormat("Gaussian Cube field", ".cube", ReadDataFormatCUBE),
    FileFormat("Portable Network Graphics", ".png", nullptr, WriteImageFormatPNG),
    FileFormat() // invalid format
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
FrameStructure::FrameStructure(QWidget *parent) : QTabWidget(parent), viewMol_(new ViewMolecule(this)), editSrc_(new EditMarkLines(this))
{
  this->addTab(viewMol_, tr("Structure view"));
  this->addTab(editSrc_, tr("Atomic content"));
  //
  viewMol_->mapMolecule(molecule_);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
FrameStructure::~FrameStructure()
{
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameStructure::isModified() const { return bChanged_; }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameStructure::setModified(bool bSet) { bChanged_ = bSet; }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
Path FrameStructure::getPath(void) const { return path_source_; }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief //
/// @param //
//
bool FrameStructure::hasPath(void) const { return path_source_.empty() ? false : true; }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
Path FrameStructure::resetPath(Path path_new)
{
  std::swap(path_new, path_source_);
  return path_new;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
ViewMolecule *FrameStructure::getMoleculeView() const
{
  return viewMol_;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
MapMolecule *FrameStructure::getMoleculeMap() const
{
  return viewMol_->getMolMap();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
Molecule *FrameStructure::getMolecule() const
{
  return viewMol_->getMolecule();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameStructure::loadFile()
{
  //
  this->exportFromPath(); // reloading
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameStructure::exportFromPath(Path the_path)
{
  bool bToModify(false);
  if (the_path.empty())
  {
    if (!this->hasPath())
      return false;
    the_path = this->getPath();
  }
  else
  {
    bToModify = bool(the_path.compare(path_source_));
    // if paths are equal, this is reload, with substitution otherwise
  }

  std::string str_ext = the_path.extension().string();
  bool bRes = false;
  auto *pMol = this->getMolecule();

  // Cast the format file object from the extension: the FormatTag pattern
  // deserves a special mechanism to realize
  if (!str_ext.compare(".xyz")) // ReadFileFormat<AcquireFileXYZ>(* * *)
    bRes = ReadDataFormatXYZ(the_path, *this);

  if (!str_ext.compare(".cml")) // ReadFileFormat<AcquireFileCML>(* * *)
    bRes = ReadDataFormatCML(the_path, *this);

  // if(!ex.compare(".hin")) ReadFileFormat<AcquireFileHIN>(* * *)
  // if(!ex.compare(".mgp")) ReadFileFormat<AcquireFileMGP>(* * *)
  if (!str_ext.compare(".pdb")) // ReadFileFormat<AcquireFilePDB>(* * *)
    bRes = ReadDataFormatPDB(the_path, *this);

  // if(!ex.compare(".sum")) ReadFileFormat<AcquireFileSUM>(* * *)
  // if(!ex.compare(".wfn")) ReadFileFormat<AcquireFileWFN>(* * *)
  // if(!ex.compare(".wfx")) ReadFileFormat<AcquireFileWFN>(* * *)
  if (!str_ext.compare(".cube")) // ReadFileFormat<AcquireFileCUBE>(* * *)
    bRes = ReadDataFormatCUBE(the_path, *this);

  // if(!ex.compare(".mol2")) ReadFileFormat<AcquireFileMOL2>(* * *)
  // if(!ex.compare(".extout")) ReadFileFormat<AcquireFileExtOut>(* * *)
  // ^^^^^^^^^^^^^^^^^ formats enumeration ^^^^^^^^^^^^^^^^^
  if (bRes)
  {
    viewMol_->showMolecule(nullptr, true);
    this->setModified(bToModify);
  }
  return bRes;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameStructure::clearContent(void)
{
  this->getMolecule()->Initialize();
  viewMol_->showMolecule(nullptr, false);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
