#ifndef __Frame_Document_h__
#define __Frame_Document_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QTabWidget>

#include <QPointer>
#include <QPrinter>
#include <QString>
#include <QMap>

#include <QTextDocument>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include "Molecule.h"
#include "MakeBondsDist.h"

#include "NamedOperation.h"

#include "AcquireMoleculeFile.h"

#include "TextSource.h"
#include "ViewMolecule.h"

typedef vtkSmartPointer<Molecule> AMolecule;
typedef vtkNew<Molecule> NewMolecule;

using namespace vtk;

#include <map>

using namespace std;

class FrameDocument
    : public QTabWidget
{
    Q_OBJECT
public:
    typedef NamedOperationT<FrameDocument, QString> NamedDocOperation;

    // static NamedDocOperation Inputs[];

    typedef NamedFileFormatT<FrameDocument, QString> FileFormat;
    typedef QMap<QString, FileFormat> FileFormatMap;
    static const FileFormatMap MapFileFormats;

    static QString filterForOpen(void);
    static QString filterForSave(void);
    static QString filterForExport(void);

    explicit FrameDocument(QWidget * /*parent*/ = nullptr);
    ~FrameDocument() override = default;

    TextSource *getTextSource() const { return (idTextSrc_ < 0) ? nullptr : textSrc_; }
    TextSource *editSource();
    ViewMolecule *getViewMolecule() const { return (idViewMol_ < 0) ? nullptr : viewMol_; }
    ViewMolecule *viewMolecule();

    QTextDocument *documentAtoms();

    void setSourceModified(bool /* b */ = true);
    bool isSourceModified() const;

    bool saveSourceFile(const QString & /* fullpath */, QString /*fmt*/ = QString());
    bool saveSource(QIODevice *pDev);

    bool openSourceFilePath(const QString & /*fullpath*/, QString /*fmt*/ = QString());
    bool saveContentFilePath(const QString & /*path*/, QString /*fmt*/ = QString());
    // bool openSourceFile(QIODevice* in, const QString& fmt);

    void insertAtomicText(const QString & /*src*/);

    void reviewMolecule(void);

    Molecule* molecule() const {return molecule_;}

public slots:
    void doPrintContent(QPrinter * /*printer*/);

protected:
    void setLinearAngstrom();
    void setLinearBohr();
    void setLinearPicometer();
    // ========================================================================
    bool loadDataFile(const QString & /*path*/);
    // Complete intrinsic format file
    template <class MoleculeReader>
    bool loadMoleculeFile(const QString &path)
    {
        if (path.isEmpty())
            return false;
        QByteArray arg = path.toLocal8Bit();

        NewMolecule newmol;

        vtkNew<MoleculeReader> read_file;
        read_file->SetFileName(arg.data());
        read_file->SetOutput(newmol);
        read_file->Update();

        molecule_->DeepCopy(newmol);

        return bool(molecule_->GetNumberOfAtoms() > 0);
    }
    //
    //
    bool loadFileCUBE(const QString & /*path*/);
    bool loadFileEXTOUT(const QString & /*path*/);
    bool loadFileMGP(const QString & /*path*/);
    bool loadFileSUM(const QString & /*path*/);
    bool loadFileWFN(const QString & /*path*/);
    bool loadFileWFX(const QString & /*path*/);
    bool loadFileXYZ(const QString & /*path*/);
    //
    // ========================================================================
    //
    bool saveDataFile(const QString & /*path*/);
    // Complete intrinsic format file
    bool saveFileXYZ(const QString & /*path*/);
    // template image file writer:
    template <class ImageWriter>
    bool saveImageFile(const QString &path)
    {
        if (path.isEmpty())
            return false;
        QByteArray bytes = path.toLocal8Bit();
        vtkNew<ImageWriter> write;
        write->SetFileName(bytes.data());
        // this->SetupImageWriter(write.Get());
        this->getViewMolecule()->viewStructure()->exportImageTo(write);
        return true; //
    }
    bool saveFileBMP(const QString & /*path*/);
    bool saveFilePS(const QString & /*path*/);
    bool saveFileJPEG(const QString & /*path*/);
    bool saveFilePNG(const QString & /*path*/);
    bool saveFileTIFF(const QString & /*path*/);
    //
    // ========================================================================

private slots:

    int readAtomsFromSource(int /*nBlocks*/);

private:
    static const QString filterAll;
    /* text source */
    int idTextSrc_ = -1;
    QPointer<TextSource> textSrc_;
    /* view molecule */
    int idViewMol_ = -1;
    QPointer<ViewMolecule> viewMol_;

    // data itself by the essence
    QString title_ = QString();
    AMolecule molecule_;
};

#endif // !__Frame_Document_h__
