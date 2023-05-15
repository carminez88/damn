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
    // FIXME: hardcoded address
    m_socket = std::make_unique<socket_t>( "tcp://127.0.0.1:5555" );

    if ( not m_socket->init<ConnectPolicy>( m_context, zmq::socket_type::sub ) ) {
        spdlog::error( "Failed to initialize socket!" );
        return;
    }
    else
        spdlog::info( "Socket initialized!" );

    // Main loop
    while ( m_isRunning ) {

        // Listen on ZMQ socket with a timeout of X seconds
        // If package is found, emit it
        spdlog::info("Trying to read something...");

        if ( auto pktRet = m_socket->read(); pktRet.has_value() ) {

            auto packet = std::move( pktRet.value() );

            spdlog::info( "Received packet {}", packet.DebugString() );
            
            Q_EMIT notifyPacket( std::move( packet ) );
        }

        // Process events
        QCoreApplication::processEvents();
    }
}

void DAMNListener::on_updateRunningStatus(bool isRunning)
{
    m_isRunning = isRunning;
}

} // namespace damn
