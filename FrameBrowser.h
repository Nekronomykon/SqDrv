#ifndef __Frame_Browser_h__
#define __Frame_Browser_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QMainWindow>

#include <QProgressBar>
#include <QCloseEvent>
#include <QSessionManager>
#include <QWidget>

#include <QDoubleValidator>

#include <QPainter>
#include <QPointer>
#include <QPrinter>

#include "FrameDocument.h"

#include "ViewWorkspace.h"
// #include "ViewLogActions.h"

// #include "ViewCommandLine.h"

#include "ChooseNamedColor.h"
#include <QLineEdit>

#include "ui_FrameBrowser.h"

class FrameBrowser
    : public QMainWindow,
      private Ui_FrameBrowser
{
    Q_OBJECT
public:
    explicit FrameBrowser(QWidget * /*parent*/ = nullptr);
    ~FrameBrowser() override = default;

    static FrameBrowser *createNew(void);
    static FrameBrowser *provideFor(const QString & /* name */);

    FrameBrowser *tileAgainst(const QWidget * /*previous*/ = nullptr);
    bool navigateTo(const QString & /*namefull*/, QString /* context */ = QString());

    QString getPathBound(void) const { return path_bound_; }

protected:
    void closeEvent(QCloseEvent *event) override;

    void readSettings(void);
    void saveSettings(void);

    bool maybeSave(void);
    bool save(void);
    QString doExportAs(void);
    bool saveAs(void);
    bool isUnbound(void) const;
    bool hasPathBound(void) const;

private:
    /* operations */
    void setupActions(void);
    void setupToolBars(void);
    void setupDocking(void);
    void updateUi(void);

    enum
    {
        MaxRecentFiles = 10
    };
    QAction *actionRecent_[MaxRecentFiles] = {0};
    /* data */
    QString path_bound_ = QString();
    QPointer<ChooseNamedColor> nameBgColor_;
    QPointer<QDoubleValidator> vldColorComp_;
    QPointer<QLineEdit> editBgRed_;
    QPointer<QLineEdit> editBgGreen_;
    QPointer<QLineEdit> editBgBlue_;
    QPointer<QProgressBar> progress_;
    QPointer<ViewWorkspace> workspace_;
    // QPointer<ViewLogActions> log_;
    // QPointer<ViewCommandLine> commandline_;

    QPrinter printer_;

private slots:
    /* void */
    void navigateToRecent(void);
    void contentModified(void);
    void commitData(QSessionManager & /*mgr */);
    void doPrintDocument(QPrinter* /*printer*/);
    void onChangedClipboardData(void);

    /* background color update */
    void updateBackgroundRed(void); 
    void updateBackgroundGreen(void); 
    void updateBackgroundBlue(void); 


    /* Auto assign */
    void on_menuFile__aboutToShow(void);
    void on_menuTabbed__aboutToShow(void);
    void on_menuRecent__aboutToShow(void);

    /* File menu */
    void on_actionNew__triggered(void);
    void on_actionClone__triggered(void);
    void on_actionOpen__triggered(void);
    void on_actionForget__triggered(void);
    void on_actionForgetAll__triggered(void);
    void on_actionReload__triggered(void);
    void on_actionSave__triggered(void);
    void on_actionExport__triggered(void);
    void on_actionSaveAs__triggered(void);
    void on_actionProperties__triggered(void);
    void on_actionClose__triggered(void);
    void on_actionExit__triggered(void);

    /* Edit menu */
    void on_actionUndo__triggered(void);
    void on_actionRedo__triggered(void);
    void on_actionCut__triggered(void);
    void on_actionCopy__triggered(void);
    void on_actionPaste__triggered(void);
    void on_actionClear__triggered(void);
    void on_actionClearAll__triggered(void);

    /* View menu */
    void on_actionFullScreen__triggered(void);
    void on_actionElements__triggered(void);
    void on_actionViewStatus__triggered(void);

    /* Text Source menu */
    void on_actionTextSource__triggered(void);
    void on_actionSetSourceFont__triggered(void);

    /* View Molecule menu */
    void on_actionViewMolecule__triggered(void);
    void on_actionViewMoleculeFast__triggered(void);
    void on_actionViewMoleculeBalls__triggered(void);
    void on_actionViewMoleculeSticks__triggered(void);
    void on_actionViewMoleculeSpheres__triggered(void);

    /* Project menu */
    void on_actionProjectOrthogonal__triggered(void);
    void on_actionProjectPerspective__triggered(void);
    void on_actionProjectReset__triggered(void);

    /* Print menu */
    void on_actionPrintPreview__triggered(void);
    void on_actionPrintSetup__triggered(void);
    void on_actionPageSetup__triggered(void);
    void on_actionPrint__triggered(void);

    /* Tools menu */
    void on_actionOptions__triggered(void);

    /* Help menu */
    void on_actionAbout__triggered(void);
    void on_actionAboutQt__triggered(void);
};

#endif // !__Frame_Browser_h__
