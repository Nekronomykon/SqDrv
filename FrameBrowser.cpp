#include "FrameBrowser.h"

#include <vtkVersion.h>

#include <QMessageBox>
#include <QDockWidget>
#include <QWidgetList>
#include <QScreen>
#include <QString>

#include <QPainter>
#include <QPointer>
#include <QPrinter>

#include <QGuiApplication>
#include <QClipboard>
#include <QMimeData>

#include <QPrintDialog>
#include <QPageSetupDialog>
#include <QPrintPreviewDialog>

#include <QDateTime>

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>

#include <QFont>
#include <QFontDialog>

#include <QRectF>
#include <QSettings>

#include "ViewMolecule.h"
#include "ViewStructure.h"

#include "TableElements.h"
#include "DialogFileProperties.h"

namespace
{
    ///////////////////////////////////////////////////////////////////////
    //
    static inline QString keyRecentFiles()
    {
        return QStringLiteral("FileRecent");
    }
    //
    static inline QString keyBackColor()
    {
        return QStringLiteral("BackgroundColor");
    }
    //
    static inline QString keyGeometry()
    {
        return QStringLiteral("Geometry");
    }
    //
    static inline QString keyFile()
    {
        return QStringLiteral("File");
    }
    //
    ///////////////////////////////////////////////////////////////////////
    //
    static bool hasRecentFiles()
    {
        QSettings settings;
        const int count = settings.beginReadArray(keyRecentFiles());
        settings.endArray();
        return count > 0;
    }
    //
    static QStringList readRecentFilesFrom(QSettings &settings)
    {
        QStringList result;
        const int count = settings.beginReadArray(keyRecentFiles());
        for (int i = 0; i < count; ++i)
        {
            settings.setArrayIndex(i);
            result.append(settings.value(keyFile()).toString());
        }
        settings.endArray();
        return result;
    }
    //
    // ...defaulted
    static QStringList readRecentFiles()
    {
        QSettings settings;
        // (QCoreApplication::organizationName(), QCoreApplication::applicationName());
        QStringList result;
        const int count = settings.beginReadArray(keyRecentFiles());
        for (int i = 0; i < count; ++i)
        {
            settings.setArrayIndex(i);
            result.append(settings.value(keyFile()).toString());
        }
        settings.endArray();
        return result;
        // == return readRecentFilesFrom(settings);
    }
    //
    static void writeRecentFilesTo(const QStringList &files, QSettings &settings)
    {
        const int count = files.size();
        settings.beginWriteArray(keyRecentFiles());
        for (int i = 0; i < count; ++i)
        {
            settings.setArrayIndex(i);
            settings.setValue(keyFile(), files.at(i));
        }
        settings.endArray();
    }
    static void writeAsRecentFiles(const QStringList &files)
    {
        QSettings settings;
        // (QCoreApplication::organizationName(), QCoreApplication::applicationName());
        const int count = files.size();
        settings.beginWriteArray(keyRecentFiles());
        for (int i = 0; i < count; ++i)
        {
            settings.setArrayIndex(i);
            settings.setValue(keyFile(), files.at(i));
        }
        settings.endArray();
        // == writeRecentFilesTo(settings);
    }
    //
    ///////////////////////////////////////////////////////////////////////
}; // *anonymous*

///////////////////////////////////////////////////////////////////////
//
FrameBrowser *FrameBrowser::createNew()
{
    FrameBrowser *pNew = new FrameBrowser();

    return pNew;
}
//
///////////////////////////////////////////////////////////////////////
//
FrameBrowser *FrameBrowser::provideFor(const QString &name)
{
    if (name.isEmpty())
        return nullptr; // use a new window

    QFileInfo fi(name);
    QString namefull = fi.canonicalFilePath();
    if (namefull.isEmpty())
    {
        // From here it is assumed that file under the 'name (-> namefull)'
        // path does not exists, so we are opening a new window bound to
        // a non-existent file yet...
        // [not completely done yet]
        namefull = QDir::currentPath();
        namefull += QDir::separator();
        namefull += name;
    }

    // From here it is assumed that file under the 'name (-> namefull)'
    // path exists or could be opened, so we are searching windows...
    FrameBrowser *pFor = nullptr;
    QWidgetList all = qApp->topLevelWidgets();
    for (QWidget *pW : all)
    {
        FrameBrowser *pTop = qobject_cast<FrameBrowser *>(pW);
        if (!pTop)
            continue;
        QString bound = pTop->getPathBound();
        // QMessageBox::information(nullptr, tr("Title"), title);
        if (namefull.compare(bound))
            continue;
        pFor = pTop;
        break;
    }
    //
    // ---------------- Finally if not found, create it:
    //
    if (!pFor)
    {
        pFor = FrameBrowser::createNew();
        pFor->navigateTo(namefull);
    }
    // all done..
    return pFor;
}
//
///////////////////////////////////////////////////////////////////////
//
FrameBrowser::FrameBrowser(QWidget *parent)
    : QMainWindow(parent),
      nameBgColor_(new ChooseNamedColor(this)),
      vldColorComp_(new QDoubleValidator),
      editBgRed_(new QLineEdit(this)),
      editBgGreen_(new QLineEdit(this)),
      editBgBlue_(new QLineEdit(this)),
      progress_(new QProgressBar(this)),
      workspace_(new ViewWorkspace(this)),
      // log_(new ViewLogActions(this)),
      // commandline_(new ViewCommandLine(this)),
      files_(new ViewFiles(this)) ,
      // =========================================================== //
      printer_(QPrinter::HighResolution)
{
    // Primarily
    this->setupUi(this);

    // validator clamp
    // vldColorComp_->setNotation(QDoubleValidator::StandardNotation);
    // vldColorComp_->setRange(0.0, 1.0);
    //
    editBgRed_->setPlaceholderText(tr("[red]"));
    editBgGreen_->setPlaceholderText(tr("[green]"));
    editBgBlue_->setPlaceholderText(tr("[blue]"));

    // Setting it all up:
    this->readSettings();
    this->setupActions();
    this->setupToolBars();
    this->setupDocking();

    connect(frameDoc_->documentAtoms(), &QTextDocument::contentsChanged,
            // modified mapping of the 'changed/unsaved' attribute
            this, &FrameBrowser::contentModified);

    // QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &FrameBrowser::commitData);
    this->setUnifiedTitleAndToolBarOnMac(true);
    // Finally...
    progress_->setMaximumWidth(250);
    this->statusBar()->addPermanentWidget(progress_, 0);
    progress_->hide();
    // progress_->setMinimum(0);
    // progress_->setMaximum(100);
    // progress_->setValue(25);
    connect(QGuiApplication::clipboard(), &QClipboard::dataChanged, this, &FrameBrowser::onChangedClipboardData);
    this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
//
void FrameBrowser::updateUi(void)
{
    const bool hasPath = this->hasPathBound();
    actionSave_->setVisible(hasPath);
    actionReload_->setVisible(hasPath);
    actionClone_->setVisible(hasPath);

    ViewMolecule *pViewMol = frameDoc_->getViewMolecule();
    vtkColor3d cbg;
    if (pViewMol)
        cbg = pViewMol->viewStructure()->getBgColor();
    QString strVal(tr("%1"));
    nameBgColor_->setEnabled(!pViewMol ? false : true);
    editBgRed_->setEnabled(!pViewMol ? false : true);
    editBgRed_->setText(!pViewMol ? QString() : strVal.arg(cbg.GetRed()));
    editBgGreen_->setText(!pViewMol ? QString() : strVal.arg(cbg.GetGreen()));
    editBgBlue_->setText(!pViewMol ? QString() : strVal.arg(cbg.GetBlue()));
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::setupActions
void FrameBrowser::setupActions(void)
{
    //! [implicit tr context]
    const QIcon iconNew = QIcon::fromTheme("window-new", QIcon(":/images/New.png"));
    actionNew_->setIcon(iconNew);
    actionNew_->setShortcuts(QKeySequence::New);
    actionNew_->setStatusTip(tr("Create a new file"));

    const QIcon iconOpen = QIcon::fromTheme("document-open", QIcon(":/images/Open.png"));
    actionOpen_->setIcon(iconOpen);
    actionOpen_->setShortcuts(QKeySequence::Open);
    actionOpen_->setStatusTip(tr("Open an found file"));

    const QIcon iconReload = QIcon::fromTheme("document-revert", QIcon(":/images/Reload.png"));
    actionReload_->setIcon(iconReload);
    actionReload_->setShortcuts(QKeySequence::Refresh);
    actionReload_->setStatusTip(tr("Reload content from file"));

    menuRecent_->removeAction(actionRecentFiles_);
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        actionRecent_[i] = menuRecent_->addAction(QString(), this, &FrameBrowser::navigateToRecent);
        actionRecent_[i]->setVisible(false);
    }
    menuRecent_->addSeparator();
    menuRecent_->addAction(actionRecentFiles_);

    const QIcon iconSave = QIcon::fromTheme("document-save", QIcon(":/images/Save.png"));
    actionSave_->setIcon(iconSave);
    actionSave_->setShortcuts(QKeySequence::Save);
    actionSave_->setStatusTip(tr("Save the document to disk"));

    const QIcon iconSaveAs = QIcon::fromTheme("document-save-as", QIcon(":/images/SaveAs.png"));
    actionSaveAs_->setIcon(iconSaveAs);
    actionSaveAs_->setShortcuts(QKeySequence::SaveAs);
    actionSaveAs_->setStatusTip(tr("Save the document under a new name"));

    const QIcon iconClose = QIcon::fromTheme("window-close", QIcon(":/images/CloseFrame.png"));
    actionClose_->setIcon(iconClose);
    actionClose_->setShortcut(QKeySequence::Close);

    const QIcon iconExit = QIcon::fromTheme("application-exit", QIcon(":/images/Exit.png"));
    actionExit_->setIcon(iconExit);
    actionExit_->setShortcuts(QKeySequence::Quit);

    const QIcon iconUndo = QIcon::fromTheme("edit-undo", QIcon(":/images/Undo.png"));
    actionUndo_->setIcon(iconUndo);
    actionUndo_->setShortcut(QKeySequence::Undo);

    const QIcon iconRedo = QIcon::fromTheme("edit-redo", QIcon(":/images/Redo.png"));
    actionRedo_->setIcon(iconRedo);
    actionRedo_->setShortcut(QKeySequence::Redo);

    const QIcon iconCut = QIcon::fromTheme("edit-cut", QIcon(":/images/Cut.png"));
    actionCut_->setIcon(iconCut);
    actionCut_->setShortcuts(QKeySequence::Cut);
    actionCut_->setEnabled(false);
    connect(frameDoc_->getTextSource()->editAtoms(),
            &EditSource::copyAvailable, actionCut_, &QAction::setEnabled);

    const QIcon iconCopy = QIcon::fromTheme("edit-copy", QIcon(":/images/Copy.png"));
    actionCopy_->setIcon(iconCopy);
    actionCopy_->setShortcuts(QKeySequence::Copy);
    actionCopy_->setEnabled(false);
    connect(frameDoc_->getTextSource()->editAtoms(),
            &EditSource::copyAvailable, actionCopy_, &QAction::setEnabled);

    const QIcon iconPaste = QIcon::fromTheme("edit-paste", QIcon(":/images/Paste.png"));
    actionPaste_->setIcon(iconPaste);
    actionPaste_->setShortcuts(QKeySequence::Paste);

    const QIcon iconClear = QIcon::fromTheme("edit-clear", QIcon(":/images/Clear.png"));
    actionClear_->setIcon(iconClear);
    //actionClear_->setShortcut(QKeySequence::Clear);
    actionClear_->setEnabled(false);
    connect(frameDoc_->getTextSource()->editAtoms(),
            &EditSource::copyAvailable, actionClear_, &QAction::setEnabled);

    const QIcon iconClearAll = QIcon::fromTheme("edit-clear-all", QIcon(":/images/ClearAll.png"));
    actionClearAll_->setIcon(iconClearAll);

    const QIcon iconProps = QIcon::fromTheme("document-properties", QIcon(":/images/Options.png"));
    actionProperties_->setIcon(iconProps);

    const QIcon iconEditTxt(":/images/SrcEdit.png");
    actionTextSource_->setIcon(iconEditTxt);
    frameDoc_->setTabIcon(0, iconEditTxt);

    const QIcon iconFullScreen = QIcon::fromTheme("view-fullscreen", QIcon(":/images/FullScreen.png"));
    actionFullScreen_->setIcon(iconFullScreen);

    const QIcon iconPreview = QIcon::fromTheme("document-print-preview", QIcon(":/images/Preview.png"));
    actionPrintPreview_->setIcon(iconPreview);

    const QIcon iconCastSrc(":/images/SrcCast.png");
    actionViewMolecule_->setIcon(iconCastSrc);
    frameDoc_->setTabIcon(1, iconCastSrc);

    const QIcon iconMolFast(":/images/MolFast.png");
    actionViewMoleculeFast_->setIcon(iconMolFast);

    const QIcon iconMolBalls(":/images/MolBalls.png");
    actionViewMoleculeBalls_->setIcon(iconMolBalls);

    const QIcon iconMolStick(":/images/MolStick.png");
    actionViewMoleculeSticks_->setIcon(iconMolStick);

    const QIcon iconMolSphere(":/images/MolVDW.png");
    actionViewMoleculeSpheres_->setIcon(iconMolSphere);

    const QIcon iconAbout = QIcon::fromTheme("help-about", QIcon(":/images/Help.png"));
    actionAbout_->setIcon(iconAbout);

    const QIcon iconAboutQt = QIcon::fromTheme("system-help", QIcon(":/images/AboutKit.png"));
    actionAboutQt_->setIcon(iconAboutQt);

    const QIcon iconSetFont = QIcon::fromTheme("preferences-desktop-font"
                                               // , QIcon(":/images/SetFont.png")
    );
    actionSetSourceFont_->setIcon(iconSetFont);

    const QIcon iconOptions = QIcon::fromTheme("preferences-system"
                                               // , QIcon(":/images/Options.png")
    );
    actionOptions_->setIcon(iconOptions);
}
//
///////////////////////////////////////////////////////////////////////
///
///
void FrameBrowser::setupDocking(void)
{
    QDockWidget *pLeft = new QDockWidget(tr("Workspace"), this);
    pLeft->setWidget(workspace_);
    this->addDockWidget(Qt::LeftDockWidgetArea, pLeft);

    QDockWidget *pFiles = new QDockWidget(tr("File system"), this);
    pFiles->setWidget(files_);
    this->tabifyDockWidget(pLeft,pFiles);

    // QDockWidget *pLog = new QDockWidget(tr("Log actions"), this);
    // pLog->setWidget(log_);
    // this->addDockWidget(Qt::BottomDockWidgetArea, pLog);

    // QDockWidget *pCmdLine = new QDockWidget(tr("Command line"), this);
    // pCmdLine->setWidget(commandline_);
    // this->tabifyDockWidget(pLog, pCmdLine);
}
//
///////////////////////////////////////////////////////////////////////
///
///
void FrameBrowser::setupToolBars(void)
{
    barTools_->addAction(actionNew_);
    barTools_->addAction(actionOpen_);
    barTools_->addAction(actionSave_);

    QToolBar *tbEdit = this->addToolBar(tr("Edit"));
    tbEdit->addAction(actionUndo_);
    tbEdit->addAction(actionRedo_);
    tbEdit->addSeparator();
    tbEdit->addAction(actionCut_);
    tbEdit->addAction(actionCopy_);
    tbEdit->addAction(actionPaste_);
    tbEdit->addSeparator();
    tbEdit->addAction(actionClear_);
    tbEdit->addAction(actionClearAll_);

    QToolBar *tbText = this->addToolBar(tr("Text"));
    tbText->addAction(actionTextSource_);
    tbText->addAction(actionSetSourceFont_);

    QToolBar *tbView = this->addToolBar(tr("View"));
    tbView->addAction(actionViewMolecule_);
    tbView->addSeparator();
    tbView->addWidget(nameBgColor_);
    // connect(choose_color_, &ChooseColor::currentTextChanged, this,
    //        &FrameBrowser::setSceneBgColor);

    // editBgRed_->setValidator(vldColorComp_);
    editBgRed_->setClearButtonEnabled(true);
    // editBgGreen_->setValidator(vldColorComp_);
    editBgGreen_->setClearButtonEnabled(true);
    // editBgBlue_->setValidator(vldColorComp_);
    editBgBlue_->setClearButtonEnabled(true);

    editBgRed_->setMaximumWidth(100);
    tbView->addWidget(editBgRed_);
    connect(editBgRed_, &QLineEdit::editingFinished, this, &FrameBrowser::updateBackgroundRed);
    editBgGreen_->setMaximumWidth(100);
    tbView->addWidget(editBgGreen_);
    connect(editBgGreen_, &QLineEdit::editingFinished, this, &FrameBrowser::updateBackgroundGreen);
    editBgBlue_->setMaximumWidth(100);
    tbView->addWidget(editBgBlue_);
    connect(editBgBlue_, &QLineEdit::editingFinished, this, &FrameBrowser::updateBackgroundBlue);
    tbView->addSeparator();
    tbView->addAction(actionViewMoleculeFast_);
    tbView->addSeparator();
    tbView->addAction(actionViewMoleculeSticks_);
    tbView->addAction(actionViewMoleculeBalls_);
    tbView->addAction(actionViewMoleculeSpheres_);

    QToolBar *tbHelp = this->addToolBar(tr("Help"));
    tbHelp->addAction(actionProperties_);
    tbHelp->addAction(actionOptions_);
    tbHelp->addAction(actionAbout_);
    tbHelp->addAction(actionAboutQt_);
}
//
///////////////////////////////////////////////////////////////////////
//
FrameBrowser *FrameBrowser::tileAgainst(const QWidget *previous)
{
    if (previous)
    {
        int topFrameWidth = previous->geometry().top() - previous->pos().y();
        if (!topFrameWidth)
            topFrameWidth = 25;
        const QPoint pos = previous->pos() + 2 * QPoint(topFrameWidth, topFrameWidth);
        if (screen()->availableGeometry().contains(this->rect().bottomRight() + pos))
            this->move(pos);
    }
    return this;
}
//
void FrameBrowser::readSettings(void)
{
    QSettings settings; // (QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value(keyGeometry(), QByteArray()).toByteArray();
    if (geometry.isEmpty())
    {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    }
    else
    {
        this->restoreGeometry(geometry);
    }
}

void FrameBrowser::saveSettings(void)
{
    QSettings settings; // (QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue(keyGeometry(), this->saveGeometry());
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::isUnbound
///
bool FrameBrowser::isUnbound(void) const
{
    return path_bound_.isEmpty();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::hasPathBound
///
bool FrameBrowser::hasPathBound(void) const
{
    return !path_bound_.isEmpty();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::maybeSave()
///
bool FrameBrowser::maybeSave(void)
{
    if (!frameDoc_->isSourceModified())
        return true;
    QString msgquery(tr("The document "));
    if (this->hasPathBound())
    {
        msgquery += tr("under the path:\n");
        msgquery += this->getPathBound();
        msgquery += '\n';
    }
    msgquery += tr("is modified\nDo you want to save the changes?");
    // const QMessageBox::StandardButton buttons = QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel;
    const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("SDI"), msgquery,
                                                                 QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret)
    {
    case QMessageBox::Save:
        return this->save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::navigateTo
///
bool FrameBrowser::navigateTo(const QString &namefull, QString context)
{
    if (!path_bound_.compare(namefull))
        return true; // already opened??? How to make the Reload
    // code functioning here?

    //
    if (context.isEmpty()) // empty context is the default one
    {
        QFileInfo fi(namefull); // trying to deduce the context from the
                                // file path name suffix:
        context = fi.suffix();  // of whatever similar...
    }
    // is the file
    bool bKnown = frameDoc_->openSourceFilePath(namefull, context); // transfer
    if (bKnown)
    {
        path_bound_ = namefull;
        // update window title
        QString title(namefull);
        title += tr("[*]");
        this->setWindowTitle(title);
        //
        this->setWindowModified(false);
        //    frameDoc_->castSource();
    }
    return bKnown;
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::navigateToRecent
///
void FrameBrowser::navigateToRecent(void)
{
    const QAction *action = qobject_cast<const QAction *>(this->sender());
    if (action)
        this->navigateTo(action->data().toString());
}

//
// EVENTS:
void FrameBrowser::closeEvent(QCloseEvent *event)
{
    this->saveSettings(); // <-- here?
    if (this->maybeSave())
    {
        // this->saveSettings(); // should it be here --- or be independent of saving data (above /  below)
        event->accept();
    }
    else
    {
        event->ignore();
    }
    // this->saveSettings();  <-- here?
}
//
// SLOTS:
void FrameBrowser::commitData(QSessionManager &mgr)
{
    if (mgr.allowsInteraction())
    {
        if (!this->maybeSave())
            mgr.cancel();
    }
    else
    {
        // Non-interactive: save without asking
        if (frameDoc_->isSourceModified())
            this->save();
    }
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::save
///
bool FrameBrowser::save(void)
{
    return this->hasPathBound()
               ? frameDoc_->saveSourceFile(this->getPathBound())
               : this->saveAs();
}
//
bool FrameBrowser::saveAs(void)
{
    QString path_saved = this->doExportAs();
    if (path_saved.isEmpty())
        return false;

    // frameDoc_->setSourceModified(false); -- excessive?
    path_bound_ = path_saved;
    path_saved += tr("[*]");
    this->setWindowModified(false);
    this->setWindowTitle(path_saved);

    return true;
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowse::doExportAs
///
QString FrameBrowser::doExportAs(void)
{
    const QFileDialog::Options options = QFileDialog::DontUseNativeDialog | QFileDialog::DontUseCustomDirectoryIcons;
    QString flt_save = FrameDocument::filterForExport();
    QString flt_sel;
    QString path = QFileDialog::getSaveFileName(this, tr("Save to a file"), this->getPathBound(), flt_save, &flt_sel, options);
    if (path.isEmpty())
        return QString();
    QFileInfo fi(path);
    QString fmt = fi.suffix();
    // path = fi.canonicalFilePath();

#ifdef VTK_DEBUG_QT_MESSAGE_BOX
    QMessageBox::information(this, tr("Export information"), tr("Exporting to\n%1\nof type %2").arg(path).arg(fmt));
#endif // VTK_DEBUG_QT_MESSAGE_BOX

    return (frameDoc_->saveContentFilePath(path, fmt)) ? path : QString();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowse::doPrintMolecule
///
void FrameBrowser::doPrintDocument(QPrinter *printer)
{
    // print the page
    QPainter painter;
    painter.begin(printer);

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

    painter.end();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowse::onChangedClipboardData
///
void FrameBrowser::onChangedClipboardData(void)
{
    QClipboard *pClipBoard = QGuiApplication::clipboard();
    if (!pClipBoard)
    {
        actionPaste_->setEnabled(false);
    }
    else
    {
        const QMimeData *pData = pClipBoard->mimeData(QClipboard::Clipboard);
        actionPaste_->setEnabled(!pData ? false : pData->hasText());
    }
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowse::contentModified
///
void FrameBrowser::contentModified(void)
{
    this->setWindowModified(frameDoc_->isSourceModified());
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowse::updateBackgroundRed
///
void FrameBrowser::updateBackgroundRed(void)
{
    ViewMolecule *pViewMol = frameDoc_->getViewMolecule();
    if (!pViewMol)
        return;
    ViewStructure *pVStr = pViewMol->viewStructure();
    QVariant vText(editBgRed_->text());
    pVStr->BgColor().SetRed(vText.toDouble());
    frameDoc_->reviewMolecule();
    this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowse::updateBackgroundGreen
///
void FrameBrowser::updateBackgroundGreen(void)
{
    ViewMolecule *pViewMol = frameDoc_->getViewMolecule();
    if (!pViewMol)
        return;
    ViewStructure *pVStr = pViewMol->viewStructure();
    QVariant vText(editBgGreen_->text());
    pVStr->BgColor().SetGreen(vText.toDouble());
    frameDoc_->reviewMolecule();
    this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowse::updateBackgroundBlue
///
void FrameBrowser::updateBackgroundBlue(void)
{
    ViewMolecule *pViewMol = frameDoc_->getViewMolecule();
    if (!pViewMol)
        return;
    ViewStructure *pVStr = pViewMol->viewStructure();
    QVariant vText(editBgBlue_->text());
    pVStr->BgColor().SetBlue(vText.toDouble());
    frameDoc_->reviewMolecule();
    this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_menuFile__aboutToShow
///
void FrameBrowser::on_menuFile__aboutToShow(void)
{
    this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_menuTabbed__aboutToShow
///
void FrameBrowser::on_menuTabbed__aboutToShow(void)
{
    frameDoc_->count();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_menuRecent__aboutToShow
///
void FrameBrowser::on_menuRecent__aboutToShow(void)
{
    // menuRecent_->clear();
    const QStringList recent = readRecentFiles();
    if (!recent.isEmpty())
    {

        actionForget_->setVisible(this->hasPathBound());
        actionForgetAll_->setVisible(true);

        const int count = qMin(int(MaxRecentFiles), recent.size());

        for (int i = 0; i < MaxRecentFiles; ++i)
        {
            if (i < count)
            {
                const QString recentpath = recent.at(i);
                // --------------------------------------------------------------------
                actionRecent_[i]->setText(tr("&%1 %2").arg(i).arg(recentpath));
                actionRecent_[i]->setData(recentpath);
                actionRecent_[i]->setVisible(true);
            }
            else
            {
                actionRecent_[i]->setVisible(false);
            }
        }
        // --------------------------------------------------------------------
        actionRecentFiles_->setVisible(recent.size() > MaxRecentFiles);
    }
    else
    {
        actionForget_->setVisible(false);
        actionForgetAll_->setVisible(false);
        // --------------------------------------------------------------------
        actionRecent_[0]->setText(tr("[empty]"));
        actionRecent_[0]->setEnabled(false);
        actionRecent_[0]->setVisible(true);
        // --------------------------------------------------------------------
        actionRecentFiles_->setVisible(false);
    }
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionClose__triggered
///
void FrameBrowser::on_actionClose__triggered(void)
{
    this->close();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionExit__triggered
///
void FrameBrowser::on_actionExit__triggered(void)
{
    qApp->closeAllWindows();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionAboutQt__triggered
///
void FrameBrowser::on_actionAboutQt__triggered(void)
{
    qApp->aboutQt();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionProperties__triggered
///
void FrameBrowser::on_actionProperties__triggered(void)
{
    DialogFileProperties dlgProperties(this->getPathBound(), this);

    int kRes = dlgProperties.exec();
    if (kRes == QDialog::Accepted)
    {
        // load newly changed data
        this->updateUi();
    }
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionNew__triggered
///
void FrameBrowser::on_actionNew__triggered(void)
{
    FrameBrowser *pNew = FrameBrowser::createNew();
    pNew->tileAgainst(this)->show();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionClone__triggered
///
void FrameBrowser::on_actionClone__triggered(void)
{
    // this->resetPathBound();
    path_bound_ = QString(); // empty path...
    frameDoc_->setSourceModified();
    this->setWindowTitle(tr("clone::read[*]"));
    this->setWindowModified(true);
    this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionOpen__triggered
///
void FrameBrowser::on_actionOpen__triggered(void)
{
    QString flt_all = FrameDocument::filterForOpen();
    QString flt_sel;
    const QFileDialog::Options options = QFileDialog::DontUseNativeDialog | QFileDialog::DontUseCustomDirectoryIcons;
    QStringList all_paths = QFileDialog::getOpenFileNames(this, tr("Open file"), this->getPathBound(), flt_all, &flt_sel, options);
    if (all_paths.isEmpty())
        return; // cancelled
    // workspace.addFiles(all_paths);
    this->navigateTo(all_paths.first());
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionForget__triggered
///
void FrameBrowser::on_actionForget__triggered(void)
{
    QString path(this->getPathBound());
    QString msg(tr("Current path is:\n%1\nAre you sure to exclude it out of the history\nor to clear all the paths there?").arg(path));
    int kRes = QMessageBox::question(
        this,                                                          // parent
        tr("Cleaning the Recent Files list"),                          // title
        msg,                                                           // message
        QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Cancel // [Yes To All] button clears all paths
    );
    if (kRes == QMessageBox::Cancel)
        return;
    QStringList empty;
    if (kRes == QMessageBox::Yes)
    {
        empty = readRecentFiles();
        empty.removeAll(path);
    }
    writeAsRecentFiles(empty);
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionForgetAll__triggered
///
void FrameBrowser::on_actionForgetAll__triggered(void)
{
    if (QMessageBox::Yes != QMessageBox::question(
                                this,                                           // parent
                                tr("Clear the Recent Files history"),           // title
                                tr("Are you sure to make it to be forgotten?"), // query...
                                QMessageBox::Yes | QMessageBox::No)             // Isn't it a simple one? ;)
    )
        return;
    QStringList empty;
    writeAsRecentFiles(empty);
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionReload__triggered
///
void FrameBrowser::on_actionReload__triggered(void)
{
    // TODO: Trace and think on this function!
    QString path(this->getPathBound());
    if (this->maybeSave())
    {
        // this->doClearAll(); ?? frameDoc_->clearAll();
        this->navigateTo(path);
        // this->doReload();
    }
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionSave__triggered
///
void FrameBrowser::on_actionSave__triggered(void)
{
    this->save();
    this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionSaveCopyAs__triggered
///
void FrameBrowser::on_actionExport__triggered(void)
{
    this->doExportAs();
    // this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionSaveAs__triggered
///
void FrameBrowser::on_actionSaveAs__triggered(void)
{
    this->saveAs();
    this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionAbout__triggered
///
void FrameBrowser::on_actionAbout__triggered(void)
{
    QString message(tr("<center><b> S q D r v </b></center><br>This example demonstrates how to write single document interface (<b>SDI</b>) applications using Qt and VTK<br><br>Qt version %1-%2 from https://wwww.qt.io<br>VTK version %3 from https://vtk.org/"));

    QMessageBox::about(this, tr("About the application"),
                       message.arg(QT_VERSION_STR).arg(QT_VERSION).arg(vtkVersion::GetVTKVersionFull()));
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionUndo__triggered
///
void FrameBrowser::on_actionUndo__triggered(void)
{
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionRedo__triggered
///
void FrameBrowser::on_actionRedo__triggered(void)
{
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionCut__triggered
///
void FrameBrowser::on_actionCut__triggered(void)
{
    EditSource *pSrc = frameDoc_->editSource()->editAtoms();
    pSrc->cut();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionCopy__triggered
///
void FrameBrowser::on_actionCopy__triggered(void)
{
    EditSource *pSrc = frameDoc_->getTextSource()->editAtoms();
    pSrc->copy();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionPaste__triggered
///
void FrameBrowser::on_actionPaste__triggered(void)
{
    QClipboard *pClipBoard = QGuiApplication::clipboard();

    if (pClipBoard)
    {
        QString sPlainType(tr("plain"));
        QString sPaste = pClipBoard->text(sPlainType, QClipboard::Clipboard);
        if (!sPaste.isEmpty())
            frameDoc_->insertAtomicText(sPaste.trimmed());
    }
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionClear__triggered
///
void FrameBrowser::on_actionClear__triggered(void)
{
    EditSource *pSrc = frameDoc_->editSource()->editAtoms();
    pSrc->clear();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionClearAll__triggered
///
void FrameBrowser::on_actionClearAll__triggered(void)
{
    // frameDoc_->doClearAll();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionFullScreen__triggered
///
void FrameBrowser::on_actionFullScreen__triggered(void)
{
    ViewMolecule *pMol = frameDoc_->viewMolecule();
    if (!pMol)
        return;
    ViewStructure *pView = pMol->viewStructure();
    if (!pView)
        return;
    vtkRenderWindow *pRW = pView->renderWindow();
    bool bFS = pRW->GetFullScreen();
    pRW->SetFullScreen(!bFS);
    pRW->Render();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionTextSource__triggered
///
void FrameBrowser::on_actionTextSource__triggered(void)
{
    frameDoc_->editSource();
    this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionElements__triggered
///
void FrameBrowser::on_actionElements__triggered(void)
{
    QPointer<TableElements> table(new TableElements(this));
    table->setFont(frameDoc_->font());
    table->setGeometry(this->geometry());
    table->show();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionViewMolecule__triggered
///
void FrameBrowser::on_actionViewMolecule__triggered(void)
{
    frameDoc_->reviewMolecule();
    this->updateUi();
}
//
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionViewMoleculeFast__triggered
///
void FrameBrowser::on_actionViewMoleculeFast__triggered(void)
{
    ViewMolecule *pMol = frameDoc_->viewMolecule();
    pMol->setMoleculeVisualStyle(StyleMapMolecule::styleFast);
    frameDoc_->reviewMolecule();
    this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionViewMoleculeSticks__triggered
///
void FrameBrowser::on_actionViewMoleculeSticks__triggered(void)
{
    ViewMolecule *pMol = frameDoc_->viewMolecule();
    pMol->setMoleculeVisualStyle(StyleMapMolecule::styleStyx);
    frameDoc_->reviewMolecule();
    this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionViewMoleculeSpheres__triggered
///
void FrameBrowser::on_actionViewMoleculeSpheres__triggered(void)
{
    ViewMolecule *pMol = frameDoc_->viewMolecule();
    pMol->setMoleculeVisualStyle(StyleMapMolecule::styleCPK);
    frameDoc_->reviewMolecule();
    this->updateUi();
}
//
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionViewMoleculeBalls__triggered
///
void FrameBrowser::on_actionViewMoleculeBalls__triggered(void)
{
    ViewMolecule *pMol = frameDoc_->viewMolecule();
    pMol->setMoleculeVisualStyle(StyleMapMolecule::styleBnS);
    frameDoc_->reviewMolecule();
    this->updateUi();
}
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionViewStatus__triggered
///
void FrameBrowser::on_actionViewStatus__triggered(void)
{
    bool bViz = !this->barStatus_->isVisible();
    this->barStatus_->setVisible(bViz);
    this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionViewOptions__triggered
///
void FrameBrowser::on_actionOptions__triggered(void)
{
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionSetFont__triggered
///
void FrameBrowser::on_actionSetSourceFont__triggered(void)
{
    TextSource *pEdit = frameDoc_->editSource();
    // bool bRead = pEdit->isReadOnly();
    // pEdit->setReadOnly(true);

    bool bOk;
    QFont font_edit = QFontDialog::getFont(&bOk, pEdit->font(), this, tr("Text font"),
                                           QFontDialog::MonospacedFonts);

    if (bOk)
    {
        pEdit->setFont(font_edit);
        pEdit->editAtoms()->setFont(font_edit); // insist this
    }
    // pEdit->setReadOnly(bRead);
    this->updateUi();
}
//
//////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionProjectOrthogonal__triggered
///
void FrameBrowser::on_actionProjectOrthogonal__triggered(void)
{
    ViewMolecule *pMol = frameDoc_->viewMolecule();
    ViewStructure *pView = pMol->viewStructure();
    pView->renderWindow()->Render();
    this->updateUi();
}
//
//////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionProjectPerspective__triggered
///
void FrameBrowser::on_actionProjectPerspective__triggered(void)
{
    ViewMolecule *pMol = frameDoc_->viewMolecule();
    ViewStructure *pView = pMol->viewStructure();
    pView->ProjectPerspective();
    pView->renderWindow()->Render();
    this->updateUi();
}
//
//////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionProjectReset__triggered
///
void FrameBrowser::on_actionProjectReset__triggered(void)
{
    ViewMolecule *pMol = frameDoc_->viewMolecule();
    ViewStructure *pView = pMol->viewStructure();
    pView->ProjectParallel();
    pView->renderWindow()->Render();
    this->updateUi();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionPageSetup__triggered
///
void FrameBrowser::on_actionPageSetup__triggered(void)
{
    QPageSetupDialog page(&printer_, this);
    /* int kRes = */ page.exec();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionPrint__triggered
///
void FrameBrowser::on_actionPrint__triggered(void)
{
    QPrintDialog print(&printer_, this);
    if (print.exec() == QDialog::Accepted)
        frameDoc_->doPrintContent(&printer_);
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionPrintSetup__triggered
///
void FrameBrowser::on_actionPrintSetup__triggered(void)
{
    QPrintDialog print(&printer_, this);
    /* int kRes = */ print.exec();
}
//
///////////////////////////////////////////////////////////////////////
/// \brief FrameBrowser::on_actionPrintPreview__triggered
///
void FrameBrowser::on_actionPrintPreview__triggered(void)
{
    QPrintPreviewDialog preview(&printer_, this);
    preview.setGeometry(this->geometry());
    connect(&preview, &QPrintPreviewDialog::paintRequested, //(QPrinter *)
            frameDoc_, &FrameDocument::doPrintContent);
    // To transfer the responsibility...
    /* int kRes = */ preview.exec();
}
//
//////////////////////////////////////////////////////////////////////
