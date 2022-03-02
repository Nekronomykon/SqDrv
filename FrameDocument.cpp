#include "FrameDocument.h"

#include <QFile>
#include <QSaveFile>
#include <QFileInfo>
#include <QTextStream>

#include <QPainter>
#include <QDateTime>
#include <QMessageBox>

#include "AcquireFileCUBE.h"
#include "AcquireFileEXTOUT.h"
#include "AcquireFileMGP.h"
#include "AcquireFileSUM.h"
#include "AcquireFileWFN.h"
#include "AcquireFileXYZ.h"
using namespace vtk;

#include <vtkBMPWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkPNGWriter.h>
#include <vtkPostScriptWriter.h>
#include <vtkTIFFWriter.h>

#include <vtkIdTypeArray.h>

#include <string>
#include <sstream>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
const QString FrameDocument::filterAll = tr("All files (*.*)");
//
///////////////////////////////////////////////////////////////////////////////
//
const FrameDocument::FileFormatMap FrameDocument::MapFileFormats = {
    {tr("sqdrv"), {tr("SqDrv Molecule"), &FrameDocument::loadDataFile, &FrameDocument::saveDataFile}}, // Not ready all
    {tr("xyz"), {tr("XMol XYZ"), &FrameDocument::loadFileXYZ, &FrameDocument::saveFileXYZ}},           // Not completely ready to save
    //
    {tr("cube"), {tr("Gaussian Cube"), &FrameDocument::loadFileCUBE, nullptr}},              //
    {tr("extout"), {tr("AIMAll Extended Output"), &FrameDocument::loadFileEXTOUT, nullptr}}, //
    {tr("mgp"), {tr("AIMAll Molecular Graph"), &FrameDocument::loadFileMGP, nullptr}},       //
    {tr("sum"), {tr("AIMAll execution summary"), &FrameDocument::loadFileSUM, nullptr}},     //
    {tr("wfn"), {tr("AIMPAC wavefunction file"), &FrameDocument::loadFileWFN, nullptr}},     // Almost ready
    {tr("wfx"), {tr("Extended wavefunction file"), &FrameDocument::loadFileWFX, nullptr}},   // Not ready
    //
    {tr("bmp"), {tr("Bitmap Image"), nullptr, &FrameDocument::saveFileBMP}},                       // Almost ready
    {tr("jpeg"), {tr("Joint Photographic Experts Group"), nullptr, &FrameDocument::saveFileJPEG}}, // Almost ready
    // {tr("pdf"), {tr("Portable Document Format"), nullptr, &FrameDocument::saveFilePDF}},         // Not yet clear how to get it ready
    {tr("png"), {tr("Portable Network Graphics"), nullptr, &FrameDocument::saveFilePNG}}, // Almost ready
    // {tr("ps"), {tr("PostScript"), nullptr, &FrameDocument::saveFilePS}},               // Not yet definitely ready;
    // It requires RGB instead of used RGBA... Huge files...
    {tr("tiff"), {tr("Tagged Image Format"), nullptr, &FrameDocument::saveFileTIFF}} // Almost ready
};
//
///////////////////////////////////////////////////////////////////////////////
//
QString FrameDocument::filterForOpen(void)
{
    QString all_known(tr("All appropriate formats ("));
    QString byformat;

    FileFormatMap::const_iterator fmt = MapFileFormats.begin();
    do
    {
        const auto &val = fmt.value();

        if (val.isUnnamed() || !val.HasRead())
            continue;

        QString mask(tr("*.%1").arg(fmt.key()));

        all_known += mask;
        all_known += ' ';

        byformat += tr("%1 files (%2);;").arg(val.getName()).arg(mask);
    } while (++fmt != MapFileFormats.end());

    if (byformat.isEmpty())
        return byformat;

    // finally
    all_known = all_known.trimmed();
    all_known += ')';
    all_known += ';';
    all_known += ';';
    all_known += byformat;
    // finally
    all_known += filterAll;
    return all_known;
}
//
///////////////////////////////////////////////////////////////////////////////
//
QString FrameDocument::filterForSave(void)
{
    QString all_known(tr("All appropriate formats ("));
    QString byformat;

    FileFormatMap::const_iterator fmt = MapFileFormats.begin();
    do
    {
        const auto &val = fmt.value();

        if (val.isUnnamed() || !val.HasRead() || !val.HasSave())
            continue;

        QString mask(tr("*.%1").arg(fmt.key()));

        all_known += mask;
        all_known += ' ';

        byformat += tr("%1 files (%2);;").arg(val.getName()).arg(mask);
    } while (++fmt != MapFileFormats.end());

    if (byformat.isEmpty())
        return byformat;

    // finally
    all_known = all_known.trimmed();
    all_known += ')';
    all_known += ';';
    all_known += ';';
    all_known += byformat;
    // finally
    all_known += filterAll;
    return all_known;
}
//
///////////////////////////////////////////////////////////////////////////////
//
QString FrameDocument::filterForExport(void)
{
    QString all_known(tr("All appropriate formats ("));
    QString byformat;

    FileFormatMap::const_iterator fmt = MapFileFormats.begin();
    do
    {
        const auto &val = fmt.value();

        if (val.isUnnamed() || !val.HasSave())
            continue;

        QString mask(tr("*.%1").arg(fmt.key()));

        all_known += mask;
        all_known += ' ';

        byformat += tr("%1 files (%2);;").arg(val.getName()).arg(mask);
    } while (++fmt != MapFileFormats.end());

    if (byformat.isEmpty())
        return byformat;

    // finally
    all_known = all_known.trimmed();
    all_known += ')';
    all_known += ';';
    all_known += ';';
    all_known += byformat;
    // finally
    all_known += filterAll;
    return all_known;
}
//
///////////////////////////////////////////////////////////////////////
//
FrameDocument::FrameDocument(QWidget *parent)
    : QTabWidget(parent),               //
      textSrc_(new TextSource(this)),   // text source view
      viewMol_(new ViewMolecule(this)), // molecular structure view
      molecule_(Molecule::New())        //
{
    this->setTabPosition(QTabWidget::South);
    this->setDocumentMode(true);
    this->setTabBarAutoHide(true);

    idTextSrc_ = this->addTab(textSrc_, tr("Text source"));
    idViewMol_ = this->addTab(viewMol_, tr("View molecule"));

    textSrc_->showMolecule(molecule_, title_);
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameDocument::saveSourceFile
///
bool FrameDocument::saveSourceFile(const QString &fullpath, QString fmt)
{
    // NB: this is still the only save function.
    // What is it to do with fmt-context supplied?
    QSaveFile file(fullpath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    return this->saveSource(&file) && file.commit();
    // ???  return this->saveSource(&file) ? file.commit() : false;
}
//
//////////////////////////////////////////////////////////////////////////
//
void FrameDocument::reviewMolecule(void)
{
    ViewStructure* pVS = viewMol_->viewStructure();
    pVS->initRendering(molecule_);
    pVS->renderWindow()->Render();
}
//
//////////////////////////////////////////////////////////////////////////
//
void FrameDocument::setLinearAngstrom()
{
}
//
//////////////////////////////////////////////////////////////////////////
//
void FrameDocument::setLinearBohr()
{
}
//
//////////////////////////////////////////////////////////////////////////
//
void FrameDocument::setLinearPicometer()
{
}
//
///////////////////////////////////////////////////////////////////////////////
/// \brief FrameDocument::saveSource
///
bool FrameDocument::saveSource(QIODevice *pDev)
{
    if (!pDev)
        return false;

    QString src;
    if (!textSrc_->buildSource(src))
        return false;

    QTextStream out(pDev);
    out << src;
    this->setSourceModified(false);
    return true;
}
//
///////////////////////////////////////////////////////////////////////////////
//
void FrameDocument::setSourceModified(bool b)
{
    return textSrc_->setModified(b);
}
//
bool FrameDocument::isSourceModified() const
{
    return textSrc_->isModified();
}
//
///////////////////////////////////////////////////////////////////////////////
//
TextSource *FrameDocument::editSource()
{
    if (idTextSrc_ >= 0 && this->currentIndex() != idTextSrc_)
        this->setCurrentIndex(idTextSrc_);
    return this->getTextSource();
}
//
///////////////////////////////////////////////////////////////////////////////
//
ViewMolecule *FrameDocument::viewMolecule()
{
    if (idViewMol_ >= 0 && this->currentIndex() != idViewMol_)
        this->setCurrentIndex(idViewMol_);
    return this->getViewMolecule();
}
//
///////////////////////////////////////////////////////////////////////////////
//
QTextDocument *FrameDocument::documentAtoms()
{
    return !textSrc_ ? nullptr
                     : textSrc_->sourceAtomsDocument();
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::loadDataFile(const QString & /*path*/)
{
    // Not yet ready!!!
    return true;
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::loadFileCUBE(const QString &path)
{
#ifdef VTK_DEBUG_QT_MESSAGE_BOX
    QMessageBox::information(this, tr("Import information"), tr("Importing from CUBE:\n%1").arg(path));
#endif // VTK_DEBUG_QT_MESSAGE_BOX
    return this->loadMoleculeFile<AcquireFileCUBE>(path);
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::loadFileMGP(const QString &path)
{
#ifdef VTK_DEBUG_QT_MESSAGE_BOX
    QMessageBox::information(this, tr("Import information"), tr("Importing from MGP:\n%1").arg(path));
#endif // VTK_DEBUG_QT_MESSAGE_BOX
    return this->loadMoleculeFile<AcquireFileMGP>(path);
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::loadFileEXTOUT(const QString &path)
{
#ifdef VTK_DEBUG_QT_MESSAGE_BOX
    QMessageBox::information(this, tr("Import information"), tr("Importing from EXTOUT:\n%1").arg(path));
#endif // VTK_DEBUG_QT_MESSAGE_BOX
    return this->loadMoleculeFile<AcquireFileEXTOUT>(path);
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::loadFileSUM(const QString &path)
{
#ifdef VTK_DEBUG_QT_MESSAGE_BOX
    QMessageBox::information(this, tr("Import information"), tr("Importing from SUM:\n%1").arg(path));
#endif // VTK_DEBUG_QT_MESSAGE_BOX
    return this->loadMoleculeFile<AcquireFileSUM>(path);
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::loadFileWFN(const QString &path)
{
#ifdef VTK_DEBUG_QT_MESSAGE_BOX
    QMessageBox::information(this, tr("Import information"), tr("Importing from WFN:\n%1").arg(path));
#endif // VTK_DEBUG_QT_MESSAGE_BOX
    return this->loadMoleculeFile<AcquireFileWFN>(path);
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::loadFileWFX(const QString &path)
{
#ifndef VTK_DEBUG_QT_MESSAGE_BOX
    QMessageBox::information(this, tr("Import information"), tr("Importing from WFX:\n%1\n[not yet ready; stubbed]").arg(path));
#endif // VTK_DEBUG_QT_MESSAGE_BOX
    // Not yet ready!!!
    return false;
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::loadFileXYZ(const QString &path)
{
#ifdef VTK_DEBUG_QT_MESSAGE_BOX
    QMessageBox::information(this, tr("Import information"), tr("Importing from XYZ:\n%1").arg(path));
#endif // VTK_DEBUG_QT_MESSAGE_BOX
    return this->loadMoleculeFile<AcquireFileXYZ>(path);
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::openSourceFilePath(const QString &path, QString fmt)
{
    const auto &fileop = MapFileFormats[fmt];
    bool bRes = fileop.HasRead() ? fileop.ReadHostContent(*this, path)
                                 : false;
    if (bRes)
    { // whatever is to be done:
        textSrc_->showMolecule(molecule_, fmt);
        viewMol_->resetMolecule(molecule_);
    }
    return bRes;
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::saveDataFile(const QString & /*path*/)
{
    // Not yet ready!!!
    return true;
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::saveFileXYZ(const QString &path)
{
    // TODO: Not yet ready!!!
    TextSource *pSrc = this->editSource();

    return pSrc->editAtoms()->saveToPath(path);
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::saveFileBMP(const QString &path)
{
#ifdef VTK_DEBUG_QT_MESSAGE_BOX
    QMessageBox::information(this, tr("Export information"), tr("Exporting to\n%1").arg(path));
#endif // VTK_DEBUG_QT_MESSAGE_BOX
    return this->saveImageFile<vtkBMPWriter>(path);
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::saveFileJPEG(const QString &path)
{
#ifdef VTK_DEBUG_QT_MESSAGE_BOX
    QMessageBox::information(this, tr("Export information"), tr("Exporting to\n%1").arg(path));
#endif // VTK_DEBUG_QT_MESSAGE_BOX
    return this->saveImageFile<vtkJPEGWriter>(path);
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::saveFilePNG(const QString &path)
{
#ifdef VTK_DEBUG_QT_MESSAGE_BOX
    QMessageBox::information(this, tr("Export information"), tr("Exporting to\n%1").arg(path));
#endif // VTK_DEBUG_QT_MESSAGE_BOX
    return this->saveImageFile<vtkPNGWriter>(path);
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::saveFilePS(const QString &path)
{
#ifndef VTK_DEBUG_QT_MESSAGE_BOX
    QMessageBox::information(this, tr("Export information"), tr("Exporting to\n%1").arg(path));
#endif // VTK_DEBUG_QT_MESSAGE_BOX
    return this->saveImageFile<vtkPostScriptWriter>(path);
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::saveFileTIFF(const QString &path)
{
#ifdef VTK_DEBUG_QT_MESSAGE_BOX
    QMessageBox::information(this, tr("Export information"), tr("Exporting to\n%1").arg(path));
#endif // VTK_DEBUG_QT_MESSAGE_BOX
    return this->saveImageFile<vtkTIFFWriter>(path);
}
//
///////////////////////////////////////////////////////////////////////////////
//
bool FrameDocument::saveContentFilePath(const QString &path, QString fmt)
{
    const auto &fileop = MapFileFormats[fmt];
    bool bRes = fileop.HasSave() ? fileop.SaveHostContent(*this, path)
                                 : false;
    return bRes;
}
//
///////////////////////////////////////////////////////////////////////////////
//
void FrameDocument::insertAtomicText(const QString &src)
{
    if (src.isEmpty())
        return;
    EditSource *pSrc = this->editSource()->editAtoms();
    QTextCursor cursor = pSrc->textCursor();
    cursor.insertText(src);
}
//
///////////////////////////////////////////////////////////////////////////////
//
int FrameDocument::readAtomsFromSource(int nBlocks)
{
    int nUsed = 0;
    ATypeArray idxFrags;
    QTextDocument *src = this->documentAtoms();
    if (!src)
        return 0;
    int nAll = src->blockCount();
    if (nAll <= 0)
        return 0;
    QTextBlock block = src->firstBlock();
    if (!block.isValid())
        return 0;
    int idAtom = 0;
    int idFragment = 0;
    int nThisFrg = 0;
    do
    {
        QString txt = block.text().trimmed();
        if (txt.isEmpty())
        {
            if (nThisFrg) // new fragment break, if there are read atoms in current
            {
                if (!idFragment++) // first fragment is read here
                {
                    idxFrags = vtkIdTypeArray::New();
                }
                idxFrags->Resize(idFragment);
                idxFrags->SetValue(idFragment - 1, idAtom);
                nThisFrg = 0;
            }
        }
        else // not empty --> try to read nex atom
        {
            QByteArray src_txt = txt.toLatin1();
            string buffer_in(src_txt.data());
            istringstream inp(buffer_in);
            //  if(success in(parsing txt to atom[idAtom]))
            ++idAtom;
            ++nThisFrg;
        }
        // finally
        --nAll; // ??? //
        block = block.next();
    } while (block.isValid());
    return nUsed;
}
//
///////////////////////////////////////////////////////////////////////////////
// SLOTS:
///////////////////////////////////////////////////////////////////////////////
//
void FrameDocument::doPrintContent(QPrinter *printer)
{
    QPainter painter(printer);
    // Rect region to print...
    QRectF rcfPage = printer->pageRect(QPrinter::DevicePixel);
    int w = rcfPage.width();
    int h = rcfPage.height();
    QRect page(0, 0, w, h);

    // temporary...
    QFont font = painter.font();
    font.setPixelSize((w + h) / 100);
    painter.setFont(font);

    painter.drawText(page, Qt::AlignVCenter | Qt::AlignCenter,
                     QDateTime::currentDateTime().toString());

    page.adjust(w / 20, h / 20, -w / 20, -h / 20);
    // end of temporary..
}