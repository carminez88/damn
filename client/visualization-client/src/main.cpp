#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <future>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

#include "Controller.h"
#include "Listener.h"
#include "dummy/DummyDevicePublisher.h"
#include "QDate"

#define DAMN_START_JTHREAD_RUNNER( jthreadVar, runner ) std::jthread jthreadVar( [ &runner ](std::stop_token stoken) { runner.run( stoken ); } );
#define DAMN_STOP_JTHREAD_RUNNER( jthreadVar ) jthreadVar.request_stop();

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

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

    // Fill model with data
    Controller::instance()->model()->add_device(
        {"X24Abc003", "PC Lily", "", DeviceData::DeviceStatus::Online });

    // Create Objects
    zmq::context_t ctx{ 1 };

    damn::DAMNListener listener{ ctx };
    damn::DummyDevicePublisher dummy{ ctx };

    QObject::connect( &listener,              &damn::DAMNListener::notifyDevice, 
                      Controller::instance(), &Controller::register_device_information, Qt::QueuedConnection );

    //std::jthread listenerThread( [ &listener ](std::stop_token stoken) { listener.run( stoken ); } );
    DAMN_START_JTHREAD_RUNNER( listenerThread, listener )

    std::this_thread::sleep_for(2s);

    DAMN_START_JTHREAD_RUNNER( dummyThread, dummy )

    auto ret = app.exec();

    DAMN_STOP_JTHREAD_RUNNER( listenerThread )
    DAMN_STOP_JTHREAD_RUNNER( dummyThread )

    return ret;
}