#include "backup.h"
#include "commandLine.h"

#include "qmlguihandler.h"
#include "worldiconprovider.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

using namespace std;

namespace randomly
{

class Application : public QGuiApplication
{
public:
    using QGuiApplication::QGuiApplication;

    // I don't like passing argc and argv during construction AND calling, but that's the way I implemented the command line parsing...
    int run(int argc, char **argv, QUrl qmlRoot = {});
};

int Application::run(int argc, char **argv, QUrl qmlRoot)
{
    if (qmlRoot.isEmpty())
        return run(argc, argv, {"qrc:/main.qml"});

    auto options = parseCommandLine(argc, argv);
    options.useGUI = true;

    auto gui = new QmlGuiHandler{options};

    Backup backup(options, gui);

    QQmlApplicationEngine qml;

    qmlRegisterSingletonInstance<QmlGuiHandler>("McBackupGUI", 1, 0, "GuiHandler", gui);

    qml.addImageProvider("worldIcon", new WorldIconProvider(this, options));

    qml.load(qmlRoot);

    if(qml.rootObjects().isEmpty())
        return EXIT_FAILURE;

    return exec();
}

}

int main(int argc, char **argv)
{
    return randomly::Application{argc, argv}.run(argc, argv);
}
