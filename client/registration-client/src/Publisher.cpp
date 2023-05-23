#include <thread>
#include <chrono>
#include "Publisher.h"
using namespace std::chrono_literals;

namespace damn {

DAMNPublisher::DAMNPublisher(zmq::context_t& context)
    : m_context { context }
{
}

void DAMNPublisher::run(std::stop_token stoken)
{
    // Create ZMQ Socket on a PUB channel
    // Many publishers on different endpoints, one single subscriber
    // FIXME: hardcoded address
    m_socket = std::make_unique<socket_t>("tcp://127.0.0.1:5555");

    if (not m_socket->init<ConnectPolicy>(m_context, zmq::socket_type::pub)) {
        spdlog::error("Failed to initialize socket!");
        return;
    } else
        spdlog::info("Socket initialized!");

    // Main loop
    while ( 1 ) {

        if ( stoken.stop_requested() ) {
            spdlog::info( "Stop requested, so I'm closing..." );
            return;
        }

        if ( not m_requests.empty() ) {

            DeviceData request = m_requests.front();

            auto pkt = deviceData2Packet( request );

            if ( not m_socket->write( pkt ) )
                spdlog::error("Cannot write packet {}", pkt.DebugString() );
            else
                spdlog::info("Sent packet {}", pkt.DebugString());

            m_requests.pop();
        }

        std::this_thread::sleep_for( 1s );
    }
}

void DAMNPublisher::onRegistrationRequest(DeviceData deviceData)
{
    m_requests.emplace( deviceData );
}

Packet DAMNPublisher::deviceData2Packet(const DeviceData& deviceData)
{
    Packet pkt;
    pkt.set_source( "WS-VI-01" ); // FIXME: hardcoded, read from cmd line
    pkt.set_timestamp( std::time(nullptr) );
    pkt.set_type(Packet::PacketType::Packet_PacketType_Registration);
    pkt.set_userid( deviceData.userID );
    pkt.set_details( deviceData.activityDetails );

    return pkt;
}

} // namespace damn
