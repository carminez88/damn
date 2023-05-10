#include <QCoreApplication>
#include "spdlog/spdlog.h"
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
    m_socket = std::make_unique<socket_t>();

    if ( not m_socket->init( m_context, zmq::socket_type::sub ) ) {
        spdlog::error( "Failed to initialize socket!" );
        return;
    }

    // Main loop
    while ( m_isRunning ) {

        // Listen on ZMQ socket with a timeout of X seconds
        // If package is found, emit it
        if ( std::optional<Packet> pkt = m_socket->read(); pkt.has_value() )
            Q_EMIT notifyPacket( pkt.value() );

        // Process events
        QCoreApplication::processEvents();

    }
}

void DAMNListener::on_updateRunningStatus(bool a_isRunning)
{
    m_isRunning = a_isRunning;
}

} // namespace damn
