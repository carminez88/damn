#include <QCoreApplication>
#include <spdlog/spdlog.h>
#include "Listener.h"

namespace damn 
{
DAMNListener::DAMNListener(zmq::context_t& context)
    : m_context ( context )
{
}

void DAMNListener::run()
{
    // Create ZMQ Socket on a SUB channel
    // Many publishers on different endpoints, one single subscriber
    m_socket = std::make_unique<socket_t>( "tcp://localhost:5556" );

    // FIXME: hardcoded address
    if ( not m_socket->init<ConnectPolicy>( m_context, zmq::socket_type::sub ) ) {
        spdlog::error( "Failed to initialize socket!" );
        return;
    }

    // Main loop
    while ( m_isRunning ) {

        // Listen on ZMQ socket with a timeout of X seconds
        // If package is found, emit it
        if ( auto pkt = m_socket->read(); pkt.has_value() )
            Q_EMIT notifyPacket( pkt.value() );

        // Process events
        QCoreApplication::processEvents();

    }
}

void DAMNListener::on_updateRunningStatus(bool isRunning)
{
    m_isRunning = isRunning;
}

} // namespace damn
