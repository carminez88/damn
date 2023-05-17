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

DummyDevicePublisher::DummyDevicePublisher(zmq::context_t& context)
	: m_context(context)
{
}

void DummyDevicePublisher::run(std::stop_token stoken)
{
    // Create ZMQ Socket on a PUB channel
    // Many publishers on different endpoints, one single subscriber
    // FIXME: hardcoded address
    m_socket = std::make_unique<socket_t>("tcp://127.0.0.1:5555");

    constexpr std::size_t kSampleNumber{ 3 };
    const std::array<std::string, kSampleNumber> kSampleSources{ "WS_VI_01", "WS_VI_02", "WS_VI_03" };
    const std::array<std::string, kSampleNumber> kSampleUsers{ "Mario Rossi", "Paolo Bianchi", "Vincent Stork" };
    const std::array<std::string, kSampleNumber> kSampleDetails{"Training", "Data processing", "Compiling" };
    const std::array<Packet::PacketType, kSampleNumber - 1> kSampleType { Packet::PacketType::Packet_PacketType_REGISTRATION,  Packet::PacketType::Packet_PacketType_DISCONNECTION };

    if (not m_socket->init<BindPolicy>(m_context, zmq::socket_type::pub)) {
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
        pkt.set_source(kSampleSources.at(generateRandomIntegerNumber<std::size_t>(0, kSampleNumber-1)));
        pkt.set_timestamp( std::time( nullptr ) );
        pkt.set_type(kSampleType.at(generateRandomIntegerNumber<std::size_t>(0, kSampleNumber - 2)));
        pkt.set_userid(kSampleUsers.at(generateRandomIntegerNumber<std::size_t>(0, kSampleNumber-1)));
        pkt.set_details(kSampleDetails.at(generateRandomIntegerNumber<std::size_t>(0, kSampleNumber-1)));

        if ( not m_socket->write( pkt ) )
            spdlog::error("Cannot write packet {}", pkt.DebugString() );

        std::this_thread::sleep_for( 5s );
	}
}

} // namespace damn