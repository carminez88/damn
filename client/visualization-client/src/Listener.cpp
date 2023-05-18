#include <QCoreApplication>
#include <spdlog/spdlog.h>
#include "Listener.h"

namespace damn 
{
DAMNListener::DAMNListener(zmq::context_t& context)
    : m_context ( context )
{
}

void DAMNListener::run(std::stop_token stoken)
{
    // Create ZMQ Socket on a SUB channel
    // Many publishers on different endpoints, one single subscriber
    // FIXME: hardcoded address
    m_socket = std::make_unique<socket_t>( "tcp://127.0.0.1:5555" );

    if ( not m_socket->init<BindPolicy>( m_context, zmq::socket_type::sub ) ) {
        spdlog::error( "Failed to initialize socket!" );
        return;
    } else
        spdlog::info( "Socket initialized!" );

    // Main loop
    while ( 1 ) {

        if ( stoken.stop_requested() ) {
            spdlog::info( "Stop requested, so I'm closing..." );
            return;
        }

        // Listen on ZMQ socket with a timeout of X seconds
        // If package is found, emit it
        spdlog::info("Shhhh...I'm listening...");

        if ( auto pktRet = m_socket->read(); pktRet.has_value() ) {

            auto packet = std::move( pktRet.value() );

            spdlog::info( "Received packet {}", packet.DebugString() );
            
            if (auto dd = packed2DeviceData(packet); dd.has_value()) {
                Q_EMIT notifyDevice(dd.value());
            }

            Q_EMIT notifyPacket( std::move( packet ) );

        }

    }
}

DeviceData::DeviceStatus packetType2DeviceStatus(Packet::PacketType packetType)
{
    switch (packetType)
    {
    case Packet::PacketType::Packet_PacketType_UNDEFINED:
    case Packet::PacketType::Packet_PacketType_DISCONNECTION:
        return DeviceData::DeviceStatus::Undefined;

    case Packet::PacketType::Packet_PacketType_REGISTRATION:
    case Packet::PacketType::Packet_PacketType_HEARTBEAT:
        return DeviceData::DeviceStatus::Online;

    default:
        break;
    }

    return DeviceData::DeviceStatus::Undefined;
}

std::optional<DeviceData> DAMNListener::packed2DeviceData(const Packet& packet)
{
    // @Francesco Lamberti: build pattern per DeviceData? ha senso? forse overkill
    DeviceData dd;
    dd.set_current_user(QString::fromStdString(packet.userid()));
    dd.set_identifier(QString::fromStdString(packet.source()));
    dd.set_status(packetType2DeviceStatus(packet.type()));
    dd.set_name(QString::fromStdString(packet.source()));
    return std::make_optional(dd);
}

} // namespace damn
