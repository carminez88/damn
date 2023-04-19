#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "Controller.h"
#include "QDate"

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
        {"X24Abc001", "PC Carmine", "Carmine", DeviceData::DeviceStatus::Online});
    Controller::instance()->model()->add_device(
        {"X24Abc002", "PC Fabrizio", "", DeviceData::DeviceStatus::Connecting});
    Controller::instance()->model()->add_device(
        {"X24Abc003", "PC Lily", "", DeviceData::DeviceStatus::Undefined});
    Controller::instance()->model()->add_device(
        {"X24Abc004", "PC 3", "Lily", DeviceData::DeviceStatus::Online});
    Controller::instance()->model()->add_device(
        {"X24Abc005", "PC 4", "", DeviceData::DeviceStatus::Online});

    return app.exec();
}
