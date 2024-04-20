#include "FrameExplorer.h"

#include <vtkVersion.h>

#include "DialogFileProperties.h"

#include <QCoreApplication>
#include <QKeySequence>
#include <QToolButton>
#include <QDockWidget>
#include <QFontDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QScreen>
#include <QPoint>
#include <QDir>

namespace // anonymous for setting keys
{
  static inline QString nameNewFile(void)
  {
    return QStringLiteral(":: New file ::");
  }
  //
  ///////////////////////////////////////////////////////////////////////
  //
  static inline QString keyRecentFiles(void)
  {
    return QStringLiteral("RecentFiles");
  }
  //
  static inline QString keyRecentFile(void)
  {
    return QStringLiteral("RecentFile");
  }
  //
  static inline QString keyBackgroundColor(void)
  {
    return QStringLiteral("BackgroundColor");
  }
  //
  static inline QString keyGeometry(void)
  {
    return QStringLiteral("Geometry");
  }
  //
  static inline QString keyState(void)
  {
    return QStringLiteral("State");
  }
  //
  static inline QString keyFile(void)
  {
    return QStringLiteral("File");
  }
  //
  ///////////////////////////////////////////////////////////////////////
}; // namespace // anonymous for setting different string keys
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
FrameExplorer *FrameExplorer::createNewFrame(QWidget *put)
{
  FrameExplorer *pFrame = new FrameExplorer;
  assert(pFrame);
  if (put)
    pFrame->alignTileAt(put);
  return pFrame;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
FrameExplorer *FrameExplorer::setupActions(void)
{
  const QIcon iconAbout(QIcon::fromTheme("help-about", QIcon(":/images/About.png")));
  actionAbout_->setIcon(iconAbout);
  actionAbout_->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F1));
  actionAbout_->setStatusTip(tr("Show the concise information about the application"));

  const QIcon iconAboutQt(QIcon::fromTheme("system-help", QIcon(":/images/AboutKit.png")));
  actionAboutQt_->setIcon(iconAboutQt);
  actionAboutQt_->setShortcut(QKeySequence(Qt::SHIFT | Qt::CTRL | Qt::Key_F1));
  actionAboutQt_->setStatusTip(tr("Show the concise information on the Qt kit used for the application"));

  const QIcon iconNewWindow(QIcon::fromTheme("window-new", QIcon(":/images/NewWindow.png")));
  actionNewWindow_->setIcon(iconNewWindow);
  actionNewWindow_->setShortcut(QKeySequence(Qt::SHIFT | Qt::CTRL | Qt::Key_N));
  actionNewWindow_->setStatusTip(tr("Start a new top-level application window"));

  const QIcon iconClose(QIcon::fromTheme("window-close", QIcon(":/images/CloseFrame.png")));
  actionClose_->setIcon(iconClose);
  actionClose_->setShortcut(QKeySequence::Close);
  actionClose_->setStatusTip(tr("Close this top-level application window"));

  const QIcon iconExit(QIcon::fromTheme("application-exit", QIcon(":/images/Exit.png")));
  actionExit_->setIcon(iconExit);
  actionExit_->setShortcuts(QKeySequence::Quit);
  actionExit_->setStatusTip(tr("Close all windows and exit the application"));

  const QIcon iconFullScreen(QIcon::fromTheme("view-fullscreen", QIcon(":/images/FullScreen.png")));
  actionFullScreen_->setIcon(iconFullScreen);
  actionFullScreen_->setShortcuts(QKeySequence::FullScreen);
  actionFullScreen_->setStatusTip(tr("Toggle the full-screen view of the window"));

  const QIcon iconNew(QIcon::fromTheme("document-new", QIcon(":/images/NewDocument.png")));
  actionNew_->setIcon(iconNew);
  actionNew_->setShortcuts(QKeySequence::New);
  actionNew_->setStatusTip(tr("Recreate an empty data file"));

  const QIcon iconOpen(QIcon::fromTheme("document-open", QIcon(":/images/Open.png")));
  actionOpen_->setIcon(iconOpen);
  actionOpen_->setShortcuts(QKeySequence::Open);
  actionOpen_->setStatusTip(tr("Open a file from disk"));

  const QIcon iconReload(QIcon::fromTheme("document-revert", QIcon(":/images/Reload.png")));
  actionReload_->setIcon(iconReload);
  actionReload_->setShortcuts(QKeySequence::Refresh);
  actionReload_->setStatusTip(tr("Reload content from the currently loaded file"));

  const QIcon iconDetach(":/images/Detach.png");
  // QIcon::fromTheme("document-detach", QIcon(":/images/Detach.png"));
  actionDetach_->setIcon(iconDetach);
  actionReload_->setStatusTip(tr("Reload content from the currently loaded file"));

  const QIcon iconSave(QIcon::fromTheme("document-save", QIcon(":/images/Save.png")));
  actionSave_->setIcon(iconSave);
  actionSave_->setShortcuts(QKeySequence::Save);
  actionSave_->setStatusTip(tr("Save the document to disk"));

  const QIcon iconExport(QIcon::fromTheme("document-export", QIcon(":/images/Export.png")));
  actionExport_->setIcon(iconExport);
  // actionSaveAs_->setShortcuts(QKeySequence::SaveAs);
  actionSaveAs_->setStatusTip(tr("Export a file to disk based on the current data"));

  const QIcon iconImport(QIcon::fromTheme("document-import", QIcon(":/images/Import.png")));
  actionImport_->setIcon(iconImport);
  // actionSaveAs_->setShortcuts(QKeySequence::SaveAs);
  actionSaveAs_->setStatusTip(tr("Load data from a disk file"));

  const QIcon iconSaveAs(QIcon::fromTheme("document-save-as", QIcon(":/images/SaveAs.png")));
  actionSaveAs_->setIcon(iconSaveAs);
  actionSaveAs_->setShortcuts(QKeySequence::SaveAs);
  actionSaveAs_->setStatusTip(tr("Save the document under a newly set path"));

  // is it required, or [Export] could be good enough?
  const QIcon iconSaveCopyAs(QIcon::fromTheme("document-save-copy-as", QIcon(":/images/SaveCopyAs.png")));
  actionSaveCopyAs_->setIcon(iconSaveAs);
  // actionSaveCopyAs_->setShortcuts(QKeySequence::SaveAs);
  actionSaveAs_->setStatusTip(tr("Save the document copy under a new path"));

  const QIcon iconUndo(QIcon::fromTheme("edit-undo", QIcon(":/images/Undo.png")));
  actionUndo_->setIcon(iconUndo);
  actionUndo_->setShortcut(QKeySequence::Undo);

  const QIcon iconRedo(QIcon::fromTheme("edit-redo", QIcon(":/images/Redo.png")));
  actionRedo_->setIcon(iconRedo);
  actionRedo_->setShortcut(QKeySequence::Redo);

  const QIcon iconCut(QIcon::fromTheme("edit-cut", QIcon(":/images/Cut.png")));
  actionCut_->setIcon(iconCut);
  actionCut_->setShortcuts(QKeySequence::Cut);

  const QIcon iconCopy(QIcon::fromTheme("edit-copy", QIcon(":/images/Copy.png")));
  actionCopy_->setIcon(iconCopy);
  actionCopy_->setShortcuts(QKeySequence::Copy);

  const QIcon iconPaste(QIcon::fromTheme("edit-paste", QIcon(":/images/Paste.png")));
  actionPaste_->setIcon(iconPaste);
  actionPaste_->setShortcuts(QKeySequence::Paste);

  const QIcon iconClear(QIcon::fromTheme("edit-clear", QIcon(":/images/Clear.png")));
  actionClear_->setIcon(iconClear);
  actionClear_->setShortcut(QKeySequence::Delete);

  const QIcon iconClearAll(QIcon::fromTheme("edit-clear-all", QIcon(":/images/ClearAll.png")));
  actionClearAll_->setIcon(iconClearAll);
  // actionClear_->setShortcut(QKeySequence::ClearAll);

  const QIcon iconClearOthers(QIcon::fromTheme("edit-clear-others", QIcon(":/images/ClearOthers.png")));
  actionClearOthers_->setIcon(iconClearOthers);
  // actionClear_->setShortcut(QKeySequence::ClearAll);

  const QIcon iconProps(QIcon::fromTheme("document-properties", QIcon(":/images/Properties.png")));
  actionProperties_->setIcon(iconProps);

  const QIcon iconOptions(QIcon::fromTheme("preferences-system", QIcon(":/images/Options.png")));
  actionOptions_->setIcon(iconOptions);

  const QIcon iconCameraReset(":/images/ProjectReset.png");
  actionCameraReset_->setIcon(iconCameraReset);

  const QIcon iconProjOrtho(":/images/ProjectOrthogonal.png");
  actionProjectOrthogonal_->setIcon(iconProjOrtho);

  const QIcon iconProjPersp(":/images/ProjectPerspective.png");
  actionProjectPerspective_->setIcon(iconProjPersp);

  const QIcon iconLabelAtoms(":/images/LabelAtom.png");
  actionLabelAtoms_->setIcon(iconLabelAtoms);

  const QIcon iconViewBonds(":/images/ViewBond.png");
  actionViewBonds_->setIcon(iconViewBonds);

  const QIcon iconLabelBonds(":/images/LabelBond.png");
  actionLabelBonds_->setIcon(iconLabelBonds);

  const QIcon iconStyleFast(":/images/StyleFast.png");
  actionStyleFast_->setIcon(iconStyleFast);

  const QIcon iconStyleBall(":/images/StyleBall.png");
  actionStyleBall_->setIcon(iconStyleBall);

  const QIcon iconStyleSpace(":/images/StyleSpace.png");
  actionStyleSpace_->setIcon(iconStyleSpace);

  const QIcon iconStyleStick(":/images/StyleStick.png");
  actionStyleStick_->setIcon(iconStyleStick);

  const QIcon iconViewStr(":/images/ViewStr.png");
  actionViewStructure_->setIcon(iconViewStr);
  frameStr_->setTabIcon(0, iconViewStr);

  const QIcon iconEditSrc(":/images/EditSrc.png");
  actionViewSource_->setIcon(iconEditSrc);
  frameStr_->setTabIcon(1, iconEditSrc);

  return this;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
FrameExplorer *FrameExplorer::setupToolBars(void)
{
  barTools_->addAction(actionNew_);
  barTools_->addAction(actionNewWindow_);
  barTools_->addSeparator();
  barTools_->addAction(actionClose_);
  barTools_->addAction(actionExit_);
  barTools_->addSeparator();
  barTools_->addAction(actionOpen_);
  QMenu *menuLoad = new QMenu("Load");
  menuLoad->addAction(actionImport_);
  menuLoad->addAction(actionReload_);
  menuLoad->setDefaultAction(actionImport_);
  menuLoad->setIcon(actionImport_->icon());
  barTools_->addAction(menuLoad->menuAction());
  //
  const QIcon iconRecent(QIcon::fromTheme("document-recent", QIcon(":/images/Recent.png")));
  menuRecentFiles_->setIcon(iconRecent);
  barTools_->addAction(menuRecentFiles_->menuAction());
  //
  QMenu *menuSave = new QMenu("Save");
  menuSave->addAction(actionSave_);
  menuSave->addAction(actionSaveAs_);
  menuSave->addSeparator();
  menuSave->addAction(actionDetach_);
  menuSave->setDefaultAction(actionSave_);
  menuSave->setIcon(actionSave_->icon());
  barTools_->addAction(menuSave->menuAction());
  barTools_->addAction(actionExport_);

  QToolBar *tbEdit = this->addToolBar(tr("Edit"));
  tbEdit->setObjectName("ToolsEdit");
  tbEdit->addAction(actionUndo_);
  tbEdit->addAction(actionRedo_);
  tbEdit->addSeparator();
  tbEdit->addAction(actionCut_);
  tbEdit->addAction(actionCopy_);
  tbEdit->addAction(actionPaste_);
  tbEdit->addSeparator();
  tbEdit->addAction(actionClear_);
  tbEdit->addAction(actionClearAll_);

  // QString sVal;
  QToolBar *tbView = this->addToolBar(tr("View"));
  tbView->setObjectName("ToolsView");
  tbView->addAction(actionFullScreen_);
  //
  tbView->addSeparator();
  //
  chooseColorByName_->setMaximumWidth(125);
  tbView->addWidget(chooseColorByName_);
  //
  editBgRed_->setMaximumWidth(100);
  tbView->addWidget(editBgRed_);
  //
  editBgGreen_->setMaximumWidth(100);
  tbView->addWidget(editBgGreen_);
  //
  editBgBlue_->setMaximumWidth(100);
  tbView->addWidget(editBgBlue_);
  tbView->addSeparator();
  tbView->addAction(actionViewBonds_);
  tbView->addSeparator();
  tbView->addAction(actionLabelBonds_);
  tbView->addAction(actionLabelAtoms_);
  tbView->addSeparator();
  tbView->addAction(actionStyleFast_);
  tbView->addAction(actionStyleBall_);
  tbView->addAction(actionStyleStick_);
  tbView->addAction(actionStyleSpace_);
  tbView->addSeparator();
  tbView->addAction(actionCameraReset_);
  tbView->addAction(actionProjectOrthogonal_);
  tbView->addAction(actionProjectPerspective_);
  //
  this->showBackRGB();

  QToolBar *tbHelp = this->addToolBar(tr("Info"));
  tbHelp->setObjectName("ToolsInfo");
  tbHelp->addAction(actionProperties_);
  tbHelp->addAction(actionOptions_);
  tbHelp->addAction(actionAbout_);
  tbHelp->addAction(actionAboutQt_);
  //
  return this;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
FrameExplorer *FrameExplorer::setupDockViews(void)
{
  QDockWidget *pTab = new QDockWidget(tr("Files"), this);
  pTab->setObjectName("Dock::FileSystem");
  pTab->setWidget(files_);
  this->addDockWidget(Qt::LeftDockWidgetArea, pTab);
  //
  QDockWidget *pLeft = new QDockWidget(tr("Workspace"), this);
  pLeft->setObjectName("Dock::WorkSpace");
  pLeft->setWidget(workspace_);
  this->tabifyDockWidget(pTab, pLeft);
  //
  QDockWidget *pThird = new QDockWidget(tr("History"), this);
  pThird->setObjectName("Dock::FileHistory");
  pThird->setWidget(history_);
  this->tabifyDockWidget(pTab,pThird);
  //
  connect(files_->getFileTree(), &ViewFilesList::activated,
          this, &FrameExplorer::browserChangeActive);
  connect(files_->getFileList(), &ViewFilesList::activated,
          this, &FrameExplorer::browserChangeActive);
  //
  return this;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief //
/// @param //
FrameExplorer::FrameExplorer(QWidget *parent)
    : QMainWindow(parent),
      chooseColorByName_(new QVTKNamedColors(this)),
      editBgRed_(new QLineEdit(this)),
      editBgGreen_(new QLineEdit(this)),
      editBgBlue_(new QLineEdit(this)),
      history_(new ViewHistory(this)),
      workspace_(new ViewWorkspace(this)),
      files_(new ViewFileSystem(this))
{
  this->setupUi(this);

  connect(chooseColorByName_, &QVTKNamedColors::currentTextChanged,
          this, &FrameExplorer::on_nameBackgroundColor);
  editBgRed_->setPlaceholderText(tr("[red]"));
  connect(editBgRed_, &QLineEdit::editingFinished, this, &FrameExplorer::on_changedBackgroundRed);
  editBgGreen_->setPlaceholderText(tr("[green]"));
  connect(editBgGreen_, &QLineEdit::editingFinished, this, &FrameExplorer::on_changedBackgroundGreen);
  editBgBlue_->setPlaceholderText(tr("[blue]"));
  connect(editBgBlue_, &QLineEdit::editingFinished, this, &FrameExplorer::on_changedBackgroundBlue);

  this->setupActions()
      ->setupToolBars()
      ->setupDockViews()
      ->loadSettings();

  this->setAttribute(Qt::WA_DeleteOnClose);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::showBackRGB(void)
{
  auto *pView = frameStr_->getViewMolecule();
  editBgRed_->setText(QString::number(pView->backgroundRed()));
  editBgGreen_->setText(QString::number(pView->backgroundGreen()));
  editBgBlue_->setText(QString::number(pView->backgroundBlue()));
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameExplorer::querySaveData()
{
  QString sCaption(tr("Unsaved changes"));
  QString sQuery(tr("The content of\n"));
  if (frameStr_->hasPath())
    sQuery += frameStr_->getPath().c_str();
  else
    sQuery += nameNewFile();
  sQuery += tr("\nis changed. Would you like to save them?");
  QMessageBox::StandardButtons replies = QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel;
  if (frameStr_->hasPath())
  {
    sQuery += tr("\nPress [Reset] to set a new save path");
    replies |= QMessageBox::Reset;
  }
  QMessageBox::StandardButton kRes =
      QMessageBox::question(this, sCaption, sQuery, replies, QMessageBox::Cancel);
  if (kRes == QMessageBox::Cancel)
    return false; // abandon the further operation
  if (kRes != QMessageBox::Ignore)
  {
    QMessageBox::information(this, tr("Smart save"), QString(tr("It is not realized yet")));
    // if(kRes == QMessageBox::Save) this->doDataSave();
    // else this->doDataSaveAs();
  }
  return true;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameExplorer::queryDataSaved()
{
  return frameStr_->isModified()
             ? this->querySaveData()
             : true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
void FrameExplorer::loadSettings()
{
  QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
  this->restoreGeometry(settings.value(keyGeometry(), QByteArray()).toByteArray());
  this->restoreState(settings.value(keyState(), QByteArray()).toByteArray());
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::saveSettings()
{
  QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
  settings.setValue(keyGeometry(), this->saveGeometry());
  settings.setValue(keyState(), this->saveState());
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief //
/// @param //
bool FrameExplorer::isFullScreen(void) const { return bFullScreen_; }
void FrameExplorer::toggleFullScreen(void)
{
  // auto *pView = frameStr_->getMoleculeView();
  if (this->isFullScreen())
  {
    this->showNormal();
    this->loadSettings();
    actionFullScreen_->setShortcuts(QKeySequence::FullScreen);
  }
  else
  {
    this->saveSettings();
    this->showFullScreen();
    actionFullScreen_->setShortcuts(QKeySequence::Cancel);
  }
  bFullScreen_ = !bFullScreen_;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::adjustTitle()
{
  QString sTitle;
  Path path_test = frameStr_->getPath();
  if (!path_test.empty())
  {
    sTitle += path_test.c_str();
    // reflect in a browser
    files_->showFilePath(sTitle);
    // and in workspace editor
    // workspace_->showFilePath(sTitle);
  }
  else
    sTitle += nameNewFile();
  sTitle += "[*]";
  this->setWindowTitle(sTitle);
  this->setWindowModified(frameStr_->isModified());
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::alignTileAt(QWidget *previous)
{
  if (!previous)
    return;
  int topFrameWidth = previous->geometry().top() - previous->pos().y();
  if (!topFrameWidth)
    topFrameWidth = 40;
  const QPoint pos = previous->pos() + 2 * QPoint(topFrameWidth, topFrameWidth);
  if (this->screen()->availableGeometry().contains(rect().bottomRight() + pos))
    this->move(pos);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// EVENTS:
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::closeEvent(QCloseEvent *event)
{
  this->saveSettings(); // <-- here? here!
  if (this->queryDataSaved())
  {
    event->accept();
  }
  else
  {
    event->ignore();
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// SLOTS:
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_nameBackgroundColor(const QString &name)
{
  QByteArray ba = name.toLatin1();
  if (!isalpha(*ba))
    return;

  auto *pView = frameStr_->viewMolecule();
  pView->ColorBackgroundByName(ba.data());
  this->showBackRGB();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_changedBackgroundRed()
{
  auto *pView = frameStr_->viewMolecule();
  pView->setBackgroundRed(editBgRed_->text().toDouble());
  this->showBackRGB();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_changedBackgroundGreen()
{
  auto *pView = frameStr_->viewMolecule();
  pView->setBackgroundGreen(editBgGreen_->text().toDouble());
  this->showBackRGB();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_changedBackgroundBlue()
{
  auto *pView = frameStr_->viewMolecule();
  pView->setBackgroundBlue(editBgBlue_->text().toDouble());
  this->showBackRGB();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionExit__triggered(void)
{
  qApp->closeAllWindows();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionNewWindow__triggered(void)
{
  this->saveSettings();
  FrameExplorer *pFrame = FrameExplorer::createNewFrame(this);
  assert(pFrame);
  pFrame->show();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionClose__triggered(void)
{
  this->close();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionAbout__triggered(void)
{
  {
    QString message(tr("<center><b> S q D r v </b></center><br>This example demonstrates how to write single document interface (<b>SDI</b>) applications using Qt and VTK<br><br>Qt version %1-%2 from https://wwww.qt.io<br>VTK version %3 from https://vtk.org/"));

    QMessageBox::about(this, tr("About the application"),
                       message.arg(QT_VERSION_STR).arg(QT_VERSION).arg(vtkVersion::GetVTKVersionFull()));
  }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionAboutQt__triggered(void)
{
  qApp->aboutQt();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionProperties__triggered()
{
  // ..but if the path is set...
  DialogFileProperties dlgProperties(frameStr_->getPath(), this);

  int kRes = dlgProperties.exec();
  if (kRes == QDialog::Accepted)
  {
    // load newly changed data
    // this->updateUi();
  }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionFullScreen__triggered()
{
  this->toggleFullScreen();
  actionFullScreen_->setChecked(this->isFullScreen());
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionOpen__triggered(void)
{
  QString sCaption(tr("Open file(s)"));
  QString sDir(frameStr_->hasPath()
                   ? tr(frameStr_->getPath().c_str())
                   : QDir::currentPath());

  // collect filter string using the "all-known" mechanism:
  QString sFilter(tr(FrameStructure::listFornatsToFilter(IsFormatToLoad()).c_str()));
  QString sSelFmt;
  QFileDialog::Options options = QFileDialog::DontUseNativeDialog | QFileDialog::DontUseCustomDirectoryIcons;
  QStringList all_open = QFileDialog::getOpenFileNames(this, sCaption, sDir, sFilter, &sSelFmt, options);
  if (!all_open.empty())
    this->addFilesRange(all_open.begin(), all_open.end());
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionReload__triggered(void)
{
  if (frameStr_->getPath().empty())
    return;
  frameStr_->reloadCurrentFile();
  this->adjustTitle();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionClearAll__triggered()
{
  frameStr_->clearContent();
  this->adjustTitle();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionDetach__triggered()
{
  if (frameStr_->getPath().empty())
    return;
  frameStr_->resetPath();
  frameStr_->setModified();
  this->adjustTitle();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionImport__triggered()
{
  // this->doDataSaveAs() ????
  QString sCaption(tr("Import data from"));
  QString sDir(frameStr_->hasPath()
                   ? tr(frameStr_->getPath().c_str())
                   : QDir::currentPath());

  QString sFilter(tr(FrameStructure::listFornatsToFilter(IsFormatToLoad()).c_str()));

  QString sSelFilter;
  QFileDialog::Options options = QFileDialog::DontUseNativeDialog | QFileDialog::DontUseCustomDirectoryIcons;
  QString sPath = QFileDialog::getOpenFileName(this, sCaption, sDir, sFilter, &sSelFilter, options);
  if (sPath.isEmpty())
    return;

  //
  Path path_import(sPath.toLocal8Bit().data());

  if (!frameStr_->hasPath() || path_import.compare(frameStr_->getPath()))
  {
    if (this->queryDataSaved())
      frameStr_->importFromPath(path_import);
    frameStr_->setModified(true); // imagine success
  }
  this->adjustTitle();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionExport__triggered()
{
  QString sCaption(tr("Export data to"));
  QString sDir(frameStr_->hasPath()
                   ? tr(frameStr_->getPath().c_str())
                   : QDir::currentPath());

  QString sFilter(tr(FrameStructure::listFornatsToFilter(IsFormatToSave()).c_str()));

  QString sSelFilter;
  QFileDialog::Options options = QFileDialog::DontUseNativeDialog | QFileDialog::DontUseCustomDirectoryIcons;
  QString sPath = QFileDialog::getSaveFileName(this, sCaption, sDir, sFilter, &sSelFilter, options);
  if (sPath.isEmpty())
    return;
  // * * *
  Path path_export(sPath.toLocal8Bit().data());
  frameStr_->exportToPath(path_export);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionSaveAs__triggered()
{
  // this->doDataSaveAs() ????
  QString sCaption(tr("Save data in"));
  QString sDir(frameStr_->hasPath()
                   ? tr(frameStr_->getPath().c_str())
                   : QDir::currentPath());

  QString sFilter(tr(FrameStructure::listFornatsToFilter(IsFormatToSave()).c_str()));

  QString sSelFilter;
  QFileDialog::Options options = QFileDialog::DontUseNativeDialog | QFileDialog::DontUseCustomDirectoryIcons;
  QString sPath = QFileDialog::getSaveFileName(this, sCaption, sDir, sFilter, &sSelFilter, options);
  if (sPath.isEmpty())
    return;
  // this->resetPath(sPath, false); // no loading, conversely, saving as:
  // this->doDataSave():{frameStr_->doSave();
  //
  frameStr_->setModified(false); // imagine success
  this->adjustTitle();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionSave__triggered()
{
  if (frameStr_->isModified())
  {
    if (frameStr_->hasPath()) // this->doDataSaveAs();
      // (else ???) this->doDataSave()
      ;
    frameStr_->setModified(false); // imagine success
    this->adjustTitle();
  }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionNew__triggered()
{
  frameStr_->clearContent();
  frameStr_->resetPath();
  frameStr_->setModified(false);
  this->adjustTitle();
  this->setWindowModified(false);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionLabelAtoms__triggered()
{
  auto *pView = frameStr_->viewMolecule();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionViewBonds__triggered()
{
  auto *pView = frameStr_->viewMolecule();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionLabelBonds__triggered()
{
  auto *pView = frameStr_->viewMolecule();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionCameraReset__triggered()
{
  auto *pView = frameStr_->viewMolecule();
  pView->resetCamera()
      ->showMolecule();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionProjectOrthogonal__triggered()
{
  auto *pView = frameStr_->viewMolecule();
  pView->setProjectParallel()
      ->showMolecule();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionProjectPerspective__triggered()
{
  auto *pView = frameStr_->viewMolecule();
  pView->setProjectPerspective()
      ->showMolecule();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionStyleBall__triggered()
{
  auto *pView = frameStr_->viewMolecule();
  pView->setStyleBall()
      ->showMolecule();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionStyleFast__triggered()
{
  auto *pView = frameStr_->viewMolecule();
  pView->setStyleFast()
      ->showMolecule();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionStyleUser__triggered()
{
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionStyleSpace__triggered()
{
  auto *pView = frameStr_->viewMolecule();
  pView->setStyleSpace()
      ->showMolecule();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionStyleStick__triggered()
{
  auto *pView = frameStr_->viewMolecule();
  pView->setStyleStick()
      ->showMolecule();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionViewStructure__triggered()
{
  auto *pView = frameStr_->viewMolecule();
  pView->showMolecule();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionViewSource__triggered()
{
  auto *pSrc = frameStr_->editSource(); //
  pSrc->setFocus();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::on_actionSetFont__triggered()
{
  QFontDialog::FontDialogOption opts = QFontDialog::DontUseNativeDialog;
  auto *pSrc = frameStr_->editSource(); //
  pSrc->setFocus();
  bool bOk(false);
  QFont fnt = QFontDialog::getFont(&bOk, pSrc->getEditAtoms()->font(), this, tr("Fonf choose"), opts);
  if (bOk)
    pSrc->updateFont(fnt);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameExplorer::browserChangeActive(const QModelIndex &idx)
{
  auto *pModel = files_->getModelFiles();
  if (pModel->isDir(idx))
    return;
  Path path_new(pModel->filePath(idx).toLocal8Bit().data());
  frameStr_->loadPath(path_new);
  this->adjustTitle();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
