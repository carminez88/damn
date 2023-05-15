#include <thread>
#include <chrono>

#include "DummyDevicePublisher.h"
using namespace std::chrono_literals;

namespace damn {

DummyDevicePublisher::DummyDevicePublisher(zmq::context_t& context)
	: m_context(context)
{
}

void DummyDevicePublisher::run()
{
    // Create ZMQ Socket on a PUB channel
    // Many publishers on different endpoints, one single subscriber
    // FIXME: hardcoded address
    m_socket = std::make_unique<socket_t>("tcp://127.0.0.1:5555");

    if (not m_socket->init<BindPolicy>(m_context, zmq::socket_type::pub)) {
        spdlog::error("Failed to initialize socket!");
        return;
    } else 
        spdlog::info("Socket initialized!");

	while ( 1 ) {

        Packet pkt;
        pkt.set_source( "WS_VI_01" );
        pkt.set_timestamp( 0 );
        pkt.set_type( Packet::PacketType::Packet_PacketType_REGISTRATION );
        pkt.set_userid("Mario Rossi");
        pkt.set_details("Data processing");

        if ( not m_socket->write( pkt ) )
            spdlog::error("Cannot write packet {}", pkt.DebugString() );
        else 
            spdlog::info("Sent packet {}", pkt.DebugString());

        std::this_thread::sleep_for( 5s );
	}
}

} // namespace damn