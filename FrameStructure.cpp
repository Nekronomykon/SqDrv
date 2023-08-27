#include "FrameStructure.h"

#include <vtkMoleculeReaderBase.h>
#include <vtkMoleculeAlgorithm.h>

#include "AcquireFileCML.h"
#include "AcquireFileCUBE.h"
#include "AcquireFilePDB.h"
#include "AcquireFileXYZ.h"

#include "ExportFileBMP.h"
#include "ExportFileJPEG.h"
#include "ExportFilePNG.h"
#include "ExportFileTIFF.h"
#include "ExportFilePS.h"

#include <QMessageBox>

bool ReadDataFormatCML(Path a_path, FrameStructure &host) { return ParseFileCMLTo(a_path, host); }
bool ReadDataFormatPDB(Path a_path, FrameStructure &host) { return ParseFilePDBTo(a_path, host); }
bool ReadDataFormatXYZ(Path a_path, FrameStructure &host) { return ParseFileXYZTo(a_path, host); }
bool ReadDataFormatCUBE(Path a_path, FrameStructure &host) { return ParseFileCUBETo(a_path, host); }
//
bool WriteImageFormatBMP(FrameStructure &host, Path a_path) { return ExportToBMPFile(host, a_path); }
bool WriteImageFormatJPEG(FrameStructure &host, Path a_path) { return ExportToJPEGFile(host, a_path); }
bool WriteImageFormatPNG(FrameStructure &host, Path a_path) { return ExportToPNGFile(host, a_path); }
bool WriteImageFormatPS(FrameStructure &host, Path a_path) { return ExportToPostScriptFile(host, a_path); }
bool WriteImageFormatTIFF(FrameStructure &host, Path a_path) { return ExportToTIFFFile(host, a_path); }
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
//
const FrameStructure::FileFormat FrameStructure::formatFile[] = {
    FileFormat("Chemical Markup Language", ".cml", ReadDataFormatCML),
    FileFormat("XMol atomic data", ".xyz", ReadDataFormatXYZ),
    FileFormat("Brookhaven data bank", ".pdb", ReadDataFormatPDB),
    FileFormat("Gaussian Cube field", ".cube", ReadDataFormatCUBE),
    //
    FileFormat("Bitmap image", ".bmp", nullptr, WriteImageFormatBMP),
    FileFormat("PostScript", ".ps", nullptr, WriteImageFormatPS),
    FileFormat("Joint Photo Expert Graphics", ".jpeg", nullptr, WriteImageFormatPNG),
    FileFormat("Portable Network Graphics", ".png", nullptr, WriteImageFormatPNG),
    FileFormat("Tagged Image Format", ".tiff", nullptr, WriteImageFormatTIFF),
    FileFormat("All files") // invalid format
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
  this->importFromPath(); // reloading
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameStructure::importFromPath(Path the_path)
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

  bool bRes = false;
  //
  const FileFormat *ff = AllFormats();
  do
  {
    bRes = ff->applyReadTo(*this, the_path);
    if (bRes)
      break;
  } while ((++ff)->isValid());
  //
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
/// @name  //
/// @brief //
/// @param //
//
bool FrameStructure::exportToPath(Path the_path)
{
  bool bRes = false;
  //
  const FileFormat *ff = AllFormats();
  do
  {
    bRes = ff->applySaveTo(*this, the_path);
    if (bRes)
      break;
  } while ((++ff)->isValid());
  //
  return bRes;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
