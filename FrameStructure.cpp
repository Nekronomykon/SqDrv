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
    {".agpviz", FileFormat("AIMAll critical point viZuals", /* ReadDataFormatAGPVIZ */ nullptr, nullptr)},       // no example yet
    {".arc", FileFormat("MOPAC run archive", /* ReadDataFormatARC */ nullptr, nullptr)},                         // no example yet
    {".basviz", FileFormat("AIMAll attractor basin viZuals", /* ReadDataFormatBASVIZ */ nullptr, nullptr)},      // no example yet
    {".bmp", FileFormat("Bitmap image", nullptr, WriteImageFormatBMP)},                                          // also .dib?
    {".cml", FileFormat("Chemical Markup Language", ReadDataFormatCML, nullptr)},                                // also .c3xml ???
    {".cube", FileFormat("Gaussian Cube field", ReadDataFormatCUBE, nullptr)},                                   // to rewrite / optimize / extract more data
    {".extout", FileFormat("AIMAll extended output", ReadDataFormatEXTOUT, nullptr)},                            // classes of AcquireAIM genus
    {".hin", FileFormat("HyperChem input", ReadDataFormatHIN, nullptr)},                                         // HyperChem formats are to be studied
    {".iasviz", FileFormat("AIMAll interatomic surfaces viZuals", /* ReadDataFormatIASVIZ */ nullptr, nullptr)}, // no example yet
    {".jpeg", FileFormat("Joint Photo Expert Graphics", nullptr, WriteImageFormatJPEG)},                         // --> VTK
    {".mgp", FileFormat("AIMAll molecular graph", ReadDataFormatMGP, nullptr)},                                  // classes of AcquireAIM genus
    {".mgpviz", FileFormat("AIMAll molecular graph viZuals", /* ReadDataFormatMGPVIZ */ nullptr, nullptr)},      // no example yet
    {".mol2", FileFormat("Tripos Mol2 structure", ReadDataFormatMOL2, nullptr)},                                 // What has Sybyl done? or WTF
    {".mop", FileFormat("MOPAC input", /* ReadDataFormatMOP */ nullptr, nullptr)},                               // no example yet
    {".out", FileFormat("MOPAC output", /* ReadDataFormatOUT */ nullptr, nullptr)},                              // no example yet
    {".pdb", FileFormat("Brookhaven data bank", ReadDataFormatPDB, nullptr)},                                    // to rewrite / optimize / extract more data
    {".png", FileFormat("Portable Network Graphics", nullptr, WriteImageFormatPNG)},                             // --> VTK
    {".pov", FileFormat("Persistence Of Vision scene", nullptr, /* WriteImageFormatPOV */ nullptr)},             // no example yet
    {".ps", FileFormat("PostScript", nullptr, WriteImageFormatPS)},                                              // --> VTK
    {".tiff", FileFormat("Tagged Image Format", nullptr, WriteImageFormatTIFF)},                                 // --> VTK
    {".sum", FileFormat("AIMAll analysis summary", ReadDataFormatSUM, nullptr)},                                 // classes of AcquireAIM genus
    {".sumviz", FileFormat("AIMAll analysis summary viZuals", /* ReadDataFormatSUMVIZ */ nullptr, nullptr)},     // no example yet
    {".wfn", FileFormat("WaveFunction data", ReadDataFormatWFN, nullptr)},                                       // Parameters of Rho calx are not used
    {".wfx", FileFormat("WaveFunction eXtended data", ReadDataFormatWFX, nullptr)},                              // Parameters of Rho calx are not used
    {".xyz", FileFormat("XMol atomic data", ReadDataFormatXYZ, nullptr)},                                        // ReadAtom as the traits --> many uses of it
    {".zmt", FileFormat("HyperChem Z-matrix", /* ReadDataFormatZMT */ nullptr, nullptr)}                         // no example yet
                                                                                                                 // Moar Z-matrix readerz required!!!!
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
    //
    FileFormat(".bmp", "Bitmap image", nullptr, WriteImageFormatBMP),
    FileFormat(".ps", "PostScript", nullptr, WriteImageFormatPS),
    FileFormat(".jpeg", "Joint Photo Expert Graphics", nullptr, WriteImageFormatJPEG),
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
bool FrameStructure::isModified() const { return bChanged_; }
//
///
QStringList FrameStructure::listAllExtensions()
{
  QStringList res;
  auto itFormatTag = suffixToFormat.cbegin();
  do
  {
    QString extz("*");
    if (!itFormatTag->first.empty())
    {
      extz += itFormatTag->first.c_str();
      res << extz;
    }
    /* code */
  } while (++itFormatTag != suffixToFormat.cend());

  return res;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
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
  // Path to_read = this->resetPath();
  // this->clearAll(); // set to override any attempts to write?
  // if (this->importFromPath(to_read))
  //   this->resetPath(to_read);
  // ^^^ is this the better formulation of reloading
  this->importFromPath();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
size_t FrameStructure::countFormatsByExt(String sext)
{
  if (sext[0] != '.')
    sext.insert(sext.begin(), '.'); // ??? mandatory? or remove '.' from keys?
  return suffixToFormat.count(sext);
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
    bRes = ff->checkReadTo(*this, the_path);
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
    bRes = ff->checkSaveTo(*this, the_path);
    if (bRes)
      break;
  } while ((++ff)->isValid());
  //
  return bRes;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
