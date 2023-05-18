#include <thread>
#include <chrono>
#include <ctime>
#include <array>
#include <random>

#include "DummyDevicePublisher.h"
using namespace std::chrono_literals;

namespace damn {

template <class T>
concept integral = std::is_integral_v<T>;

template <typename T>
requires integral<T>
static T generateRandomIntegerNumber(T min, T max)
{
    std::random_device rd;
    std::mt19937 mt( rd() );

    if (min > max)
        std::swap(min, max);

    std::uniform_int_distribution<T> dist(min, max);

    return dist(mt);
}

DummyDevicePublisher::DummyDevicePublisher(DummyDetails details, zmq::context_t& context)
	: m_context (context)
    , m_details (details)
{
}

void DummyDevicePublisher::run(std::stop_token stoken)
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

	while ( 1 ) {

        if ( stoken.stop_requested() ) {
            spdlog::info("Stop requested, so I'm closing...");
            return;
        }

        Packet pkt;
        pkt.set_source(m_details.source);
        pkt.set_timestamp(std::time(nullptr));
        pkt.set_type(Packet::PacketType::Packet_PacketType_REGISTRATION);
        pkt.set_userid(m_details.user);
        pkt.set_details(m_details.details);

        if ( not m_socket->write( pkt ) )
            spdlog::error("Cannot write packet {}", pkt.DebugString() );

        std::this_thread::sleep_for( 5s );
	}
}

} // namespace damn