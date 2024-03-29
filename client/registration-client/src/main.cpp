#include <QCoreApplication>
#include <QGuiApplication>
#include <QApplication>
#include <QCommandLineParser>
#include <future>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

#include "RegistrationClientMainWindow.h"
#include "Publisher.h"

#define DAMN_START_JTHREAD_RUNNER( jthreadVar, runner ) std::jthread jthreadVar( [ &runner ](std::stop_token stoken) { runner.run( stoken ); } );
#define DAMN_STOP_JTHREAD_RUNNER( jthreadVar ) jthreadVar.request_stop();

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QCommandLineParser parser;
    QCommandLineOption deviceIdOption(QStringList() << "i" << "device-id",
                                      QCoreApplication::translate("main", "Set the device identifier <id>"),
                                      QCoreApplication::translate("main", "Device ID"));
    QCommandLineOption serverAddressOption(QStringList() << "a" << "server-address",
                                           QCoreApplication::translate("main", "Set the server address <address>"),
                                           QCoreApplication::translate("main", "Server Address"));

    parser.addOption(deviceIdOption);
    parser.addOption(serverAddressOption);
    parser.process(app);

    const auto deviceId = parser.value(deviceIdOption);

    if ( deviceId.isEmpty() ) {
        spdlog::error( "Unable to read -i option. Please run the application with -i <identifier> option!" );
        return -1;
    }

    const auto serverAddress = [&]{
        auto address = parser.value(serverAddressOption);
        if ( address.isEmpty() ) {
            address = "127.0.0.1";
            spdlog::error( "Unable to read -a option. I will be using the addrss {}.", address.toStdString() );
        }
        return address;
    }();

    // Create Objects
    zmq::context_t ctx{ 1 };

    damn::DAMNPublisher publisher { deviceId.toStdString(), serverAddress.toStdString(), ctx };

    RegistrationClientMainWindow w { deviceId };
    w.show();

    QObject::connect( &w,         &RegistrationClientMainWindow::notifyRequest,
                      &publisher, &damn::DAMNPublisher::onRequest,
                      Qt::QueuedConnection );

    DAMN_START_JTHREAD_RUNNER( pubThread, publisher )

    auto ret = app.exec();

    DAMN_STOP_JTHREAD_RUNNER( pubThread )

    return ret;
}
