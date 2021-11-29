#include <QVTKOpenGLNativeWidget.h>
#include <QVTKApplication.h>

#include <QSurfaceFormat>
#include <QCoreApplication>

#include <QCommandLineParser>
#include <QStringList>
#include <QString>

#include "FrameBrowser.h"

int main(int argc, char *argv[])
{
    QSurfaceFormat::setDefaultFormat(ViewStructure::defaultFormat());

    // QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    // QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    // QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    QVTKApplication app(argc, argv);

    QCoreApplication::setApplicationName("SqDrv Browser");
    QCoreApplication::setOrganizationName("Nekronomykon");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file(s) to open.");
    parser.process(app);

    const QStringList all_files_to_open = parser.positionalArguments();

    FrameBrowser *pFrame = nullptr;

    for (const QString &one_file : all_files_to_open)
    {
        FrameBrowser *pNew = FrameBrowser::provideFor(one_file);
        pNew->tileAgainst(pFrame)
            ->show();
        pFrame = pNew;
    }

    if (!pFrame)
        pFrame = FrameBrowser::createNew();
    pFrame->tileAgainst()
        ->show();

    return app.exec();
}
