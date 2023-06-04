#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCommandLineParser>
#include <QDate>
#include <future>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

#include "Controller.h"
#include "Listener.h"

#define DAMN_START_JTHREAD_RUNNER( jthreadVar, runner ) std::jthread jthreadVar( [ &runner ](std::stop_token stoken) { runner.run( stoken ); } );
#define DAMN_STOP_JTHREAD_RUNNER( jthreadVar ) jthreadVar.request_stop();

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    // Parse command line args
    QCommandLineParser parser;
    QCommandLineOption serverAddressOption(QStringList() << "a" << "server-address",
                                           QCoreApplication::translate("main", "Set the server address <address>"),
                                           QCoreApplication::translate("main", "Server Address"));
    parser.addOption(serverAddressOption);
    parser.process(app);

    QString serverAddress = parser.value(serverAddressOption);

    if ( serverAddress.isEmpty() ) {
        serverAddress = "127.0.0.1";
        spdlog::error( "Unable to read -a option. I will be using the addrss {}.", serverAddress.toStdString() );
    }

    // Create QML Application
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    // Registering Controller singleton
    qmlRegisterSingletonType<Controller>("com.cppeverything.devicemonitor",
                                         1,
                                         0,
                                         "Controller",
                                         [](QQmlEngine *engine,
                                            QJSEngine *scriptEngine) -> QObject * {
                                             Q_UNUSED(engine)
                                             Q_UNUSED(scriptEngine)

                                             return Controller::instance();
                                         });

    // Register DeviceData to use DeviceData::DeviceStatus enum in qml
    qmlRegisterUncreatableType<DeviceData>("com.cppeverything.devicemonitor",
                                           1,
                                           0,
                                           "DeviceData",
                                           "Exposed for enum utilization");
    engine.load(url);

    // Create Objects
    zmq::context_t ctx{ 1 };

    damn::DAMNListener listener{ serverAddress.toStdString(), ctx };

    QObject::connect( &listener,              &damn::DAMNListener::notifyDevice, 
                      Controller::instance(), &Controller::register_device_information,
                      Qt::QueuedConnection );

    DAMN_START_JTHREAD_RUNNER( listenerThread, listener )

    auto ret = app.exec();

    DAMN_STOP_JTHREAD_RUNNER( listenerThread )

    return ret;
}
