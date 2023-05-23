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
    QCommandLineOption identifier(QStringList() << "i" << "device-id",
                                  QCoreApplication::translate("main", "Set the device identifier <id>"),
                                  QCoreApplication::translate("main", "Device ID"));
    parser.addOption(identifier);
    parser.process(app);
    const QString deviceID = parser.value(identifier);

    if ( deviceID.isEmpty() ) {
        spdlog::error( "Unable to read -i option. Please run the application with -i <identifier> option!" );
        return -1;
    }

    // Create Objects
    zmq::context_t ctx{ 1 };

    damn::DAMNPublisher publisher { deviceID.toStdString(), ctx };

    RegistrationClientMainWindow w { deviceID };
    w.show();

    QObject::connect( &w,         &RegistrationClientMainWindow::notifyRequest,
                      &publisher, &damn::DAMNPublisher::onRequest,
                      Qt::QueuedConnection );

    DAMN_START_JTHREAD_RUNNER( pubThread, publisher )

    auto ret = app.exec();

    DAMN_STOP_JTHREAD_RUNNER( pubThread )

    return ret;
}
