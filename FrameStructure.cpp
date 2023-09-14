#include "FrameStructure.h"

#include <vtkMoleculeReaderBase.h>
#include <vtkMoleculeAlgorithm.h>

#include "AcquireFileCML.h"
#include "AcquireFileHIN.h"
#include "AcquireFileMGP.h"
#include "AcquireFilePDB.h"
#include "AcquireFileSUM.h"
#include "AcquireFileWFN.h"
#include "AcquireFileWFX.h"
#include "AcquireFileXYZ.h"
#include "AcquireFileCUBE.h"
#include "AcquireFileMOL2.h"
#include "AcquireFileEXTOUT.h"

#include "ExportFileBMP.h"
#include "ExportFileJPEG.h"
#include "ExportFilePNG.h"
#include "ExportFileTIFF.h"
#include "ExportFilePS.h"

#include <QMessageBox>

#include <map>

bool ReadDataFormatCML(Path a_path, FrameStructure &host) { return ParseFileCMLTo(a_path, host); }
bool ReadDataFormatHIN(Path a_path, FrameStructure &host) { return ParseFileHINTo(a_path, host); }
bool ReadDataFormatMGP(Path a_path, FrameStructure &host) { return ParseFileMGPTo(a_path, host); }
bool ReadDataFormatPDB(Path a_path, FrameStructure &host) { return ParseFilePDBTo(a_path, host); }
bool ReadDataFormatSUM(Path a_path, FrameStructure &host) { return ParseFileSUMTo(a_path, host); }
bool ReadDataFormatWFN(Path a_path, FrameStructure &host) { return ParseFileWFNTo(a_path, host); }
bool ReadDataFormatWFX(Path a_path, FrameStructure &host) { return ParseFileWFXTo(a_path, host); }
bool ReadDataFormatXYZ(Path a_path, FrameStructure &host) { return ParseFileXYZTo(a_path, host); }
bool ReadDataFormatCUBE(Path a_path, FrameStructure &host) { return ParseFileCUBETo(a_path, host); }
bool ReadDataFormatMOL2(Path a_path, FrameStructure &host) { return ParseFileMOL2To(a_path, host); }
bool ReadDataFormatEXTOUT(Path a_path, FrameStructure &host) { return ParseFileEXTOUTTo(a_path, host); }
//
bool WriteImageFormatPS(FrameStructure &host, Path a_path) { return ExportToPostScriptFile(host, a_path); }
bool WriteImageFormatBMP(FrameStructure &host, Path a_path) { return ExportToBMPFile(host, a_path); }
bool WriteImageFormatPNG(FrameStructure &host, Path a_path) { return ExportToPNGFile(host, a_path); }
bool WriteImageFormatJPEG(FrameStructure &host, Path a_path) { return ExportToJPEGFile(host, a_path); }
bool WriteImageFormatTIFF(FrameStructure &host, Path a_path) { return ExportToTIFFFile(host, a_path); }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
//
const FrameStructure::MolFormatMap FrameStructure::suffixToFormat = {
    {".xyz", FileFormat("XMol atomic data", ReadDataFormatXYZ, nullptr)},
    {".cml", FileFormat("Chemical Markup Language", ReadDataFormatCML, nullptr)},
    {".hin", FileFormat("HyperChem input", ReadDataFormatHIN, nullptr)},
    //
};
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
//
const FrameStructure::FileFormat FrameStructure::formatFile[] = {
    FileFormat(".xyz", "XMol atomic data", ReadDataFormatXYZ, nullptr),
    FileFormat(".cml", "Chemical Markup Language", ReadDataFormatCML, nullptr), // to rewrite / optimize / extract more data
    FileFormat(".hin", "HyperChem input", ReadDataFormatHIN, nullptr),
    FileFormat(".mgp", "AIMAll molecular graph", ReadDataFormatMGP, nullptr),
    FileFormat(".sum", "AIMAll analysis summary", ReadDataFormatSUM, nullptr),
    FileFormat(".pdb", "Brookhaven data bank", ReadDataFormatPDB, nullptr), // to rewrite / optimize / extract more data
    FileFormat(".wfn", "Wavefunction data", ReadDataFormatWFN, nullptr),
    FileFormat(".wfx", "Wavefunction eXtended data", ReadDataFormatWFX, nullptr),
    FileFormat(".cube", "Gaussian Cube field", ReadDataFormatCUBE, nullptr), // to rewrite / optimize / extract more data
    FileFormat(".mol2", "Tripos Mol2 structure", ReadDataFormatMOL2, nullptr),
    FileFormat(".extout", "AIMAll extended output", ReadDataFormatEXTOUT, nullptr),
    FileFormat(".arc", "MOPAC run archive", /*ReadDataFormatARC */ nullptr, nullptr),
    FileFormat(".mop", "MOPAC input", /*, ReadDataFormatMOP */ nullptr, nullptr),
    FileFormat(".out", "MOPAC output", /*, ReadDataFormatOUT */ nullptr, nullptr),
    //
    FileFormat(".bmp", "Bitmap image", nullptr, WriteImageFormatBMP),
    FileFormat(".ps", "PostScript", nullptr, WriteImageFormatPS),
    FileFormat(".jpeg", "Joint Photo Expert Graphics", nullptr, WriteImageFormatPNG),
    FileFormat(".png", "Portable Network Graphics", nullptr, WriteImageFormatPNG),
    FileFormat(".tiff", "Tagged Image Format", nullptr, WriteImageFormatTIFF),
    FileFormat("") // invalid format to complete
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
FrameStructure::FrameStructure(QWidget *parent) : QTabWidget(parent), viewMol_(new ViewMolecule(this)), editSrc_(new EditSource(this))
{
  this->addTab(viewMol_, tr("Structure view"));
  this->addTab(editSrc_, tr("Atomic content"));
  //
  viewMol_->mapMolecule(molecule_);
  editSrc_->resetMolecule(molecule_);
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
EditSource *FrameStructure::editSource()
{
  this->setCurrentWidget(editSrc_);
  return editSrc_;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
EditSource *FrameStructure::getEditSource() const
{
  return editSrc_;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
ViewMolecule *FrameStructure::getViewMolecule(void) const
{
  return viewMol_;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
ViewMolecule *FrameStructure::viewMolecule(void)
{
  this->setCurrentWidget(viewMol_);
  return viewMol_;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
MapMolecule *FrameStructure::getMoleculeMap(void) const
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
void FrameStructure::resetTitle(String title)
{
  QString strTitle(title.c_str());
  editSrc_->resetTitleString(strTitle);
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
    bToModify = bool(the_path.compare(this->getPath()));
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
    editSrc_->showMolecule();
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
