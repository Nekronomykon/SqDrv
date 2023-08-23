#include <QVTKApplication.h>

#include <QSurfaceFormat>
#include <QCoreApplication>

#include "ViewMolecule.h"
#include "FrameExplorer.h"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
  QSurfaceFormat::setDefaultFormat(ViewMolecule::defaultFormat());

  QVTKApplication app(argc, argv);
  // VTK-speccific application part ends...
  //
  QApplication::setQuitOnLastWindowClosed(true);
  // set false if one would make use of
  // the tray activation system. Who would dare?
  // --------------------------------------------
  QCoreApplication::setOrganizationName("Nekronomykon");
  QCoreApplication::setApplicationName("SqDrv -- text-to-structure (and probably vice versa) editor");
  QCoreApplication::setApplicationVersion(QT_VERSION_STR);

  QCommandLineParser parser;
  parser.setApplicationDescription(QCoreApplication::applicationName());
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addPositionalArgument("file(s)", "One or more files to open in the workspace");
  parser.process(app);

  FrameExplorer *pFrame = FrameExplorer::createNewFrame(nullptr);
  QStringList posArgs = parser.positionalArguments();
  if (!posArgs.isEmpty())
  {
    pFrame->addFilesRange(posArgs.begin(), posArgs.end());
  }
  pFrame->show();

  return app.exec();
}
