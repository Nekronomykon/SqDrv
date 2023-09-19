#ifndef Frame_Explorer_h__
#define Frame_Explorer_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <algorithm>

#include <QWidget>
#include <QCloseEvent>

#include <QLineEdit>
#include <QFileInfo>

#include "QVTKNamedColors.h"

#include "ViewWorkspace.h"
#include "ViewFileSystem.h"
#include "ResetCursor.h"

#include "ui_FrameExplorer.h"

class FrameExplorer
    : public QMainWindow,
      protected Ui_FrameExplorer
{
  Q_OBJECT
public:
  explicit FrameExplorer(QWidget * /*parent*/ = nullptr);

  static FrameExplorer *createNewFrame(QWidget * /*put*/ = nullptr);

  template <class Iter>
  void addFilesRange(Iter from, Iter to)
  {
    if (from == to)
      return;
    Path path_to_open;
    do
    {
      QFileInfo fi(*from);
      path_to_open = fi.filesystemCanonicalFilePath();
      /*
      if (workspace_->findPath(path_to_open))
        workspace_->addPath(path_to_open);
         */
    } while (++from != to);
    ///
    ///
    frameStr_->loadPath(path_to_open);
    this->adjustTitle();
  }
  //
  void closeEvent(QCloseEvent *event) override;

protected:
  void showBackRGB(void);
  //
  void alignTileAt(QWidget * /*previous*/ = nullptr);
  //
  void loadSettings(void);
  void saveSettings(void);
  //
  bool isFullScreen(void) const;
  void toggleFullScreen(void);
  //
  bool querySaveData(void);
  bool queryDataSaved(void);
  void adjustTitle(void);
  //
  FrameExplorer *setupActions(void);
  FrameExplorer *setupToolBars(void);
  FrameExplorer *setupDockViews(void);

protected slots:
  void on_actionNewWindow__triggered(void);
  void on_actionClose__triggered(void);
  void on_actionExit__triggered(void);
  void on_actionAbout__triggered(void);
  void on_actionAboutQt__triggered(void);
  void on_actionProperties__triggered(void);
  void on_actionFullScreen__triggered(void);
  void on_actionReload__triggered(void);
  void on_actionClearAll__triggered(void);
  void on_actionDetach__triggered(void);
  void on_actionImport__triggered(void);
  void on_actionExport__triggered(void);
  void on_actionSaveAs__triggered(void);
  void on_actionSave__triggered(void);
  void on_actionOpen__triggered(void);
  void on_actionNew__triggered(void);
  void on_actionLabelAtoms__triggered(void);
  void on_actionViewBonds__triggered(void);
  void on_actionLabelBonds__triggered(void);
  void on_actionCameraReset__triggered(void);
  void on_actionProjectOrthogonal__triggered(void);
  void on_actionProjectPerspective__triggered(void);
  void on_actionStyleBall__triggered(void);
  void on_actionStyleFast__triggered(void);
  void on_actionStyleUser__triggered(void);
  void on_actionStyleSpace__triggered(void);
  void on_actionStyleStick__triggered(void);
  void on_actionViewStructure__triggered(void);
  void on_actionViewSource__triggered(void);
  void on_actionSetFont__triggered(void);

private:
  bool bFullScreen_ = false;
  //
  QPointer<QVTKNamedColors> chooseColorByName_;
  QPointer<QLineEdit> editBgRed_;
  QPointer<QLineEdit> editBgGreen_;
  QPointer<QLineEdit> editBgBlue_;
  //
  QPointer<ViewWorkspace> workspace_;
  QPointer<ViewFileSystem> files_;
private slots:
  void browserChangeActive(const QModelIndex &);
  void on_nameBackgroundColor(const QString & /* name */);
  void on_changedBackgroundRed(void);
  void on_changedBackgroundGreen(void);
  void on_changedBackgroundBlue(void);
};

#endif // !Frame_Explorer_h__
