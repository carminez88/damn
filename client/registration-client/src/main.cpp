#include <QCoreApplication>
#include <QGuiApplication>
#include <QApplication>
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

    // Create Objects
    zmq::context_t ctx{ 1 };

    damn::DAMNPublisher publisher { ctx };

    RegistrationClientMainWindow w;
    w.show();

    QObject::connect( &w, &RegistrationClientMainWindow::notifyRegistrationRequest, &publisher, &damn::DAMNPublisher::onRegistrationRequest, Qt::QueuedConnection );

    DAMN_START_JTHREAD_RUNNER( pubThread, publisher )

    auto ret = app.exec();

    DAMN_STOP_JTHREAD_RUNNER( pubThread )

    return ret;
}
