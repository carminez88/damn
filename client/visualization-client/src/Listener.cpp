#include <QCoreApplication>
#include <spdlog/spdlog.h>
#include "Listener.h"

namespace damn 
{

bool DAMNListener::createSocket()
{
    // FIXME: hardcoded address
    return createSocketImpl<ConnectInitializer<TcpAddressFormatter>>( net_data_t{ "127.0.0.1", 5556 }, zmq::socket_type::sub );
}

void DAMNListener::loopTask()
{
    // Listen on ZMQ socket with a timeout of X seconds
    // If package is found, emit it
    spdlog::info("Shhhh...I'm listening...");

    if ( auto pktRet = m_socket->read(); pktRet.has_value() ) {

        auto packet = std::move( pktRet.value() );

        spdlog::debug( "Received packet {}", packet.toString() );

        if (auto dd = packed2DeviceData(packet); dd.has_value()) {
            emit notifyDevice(dd.value());
        }

        emit notifyPacket( std::move( packet ) );

    }
}

DeviceData::DeviceStatus packetType2DeviceStatus(PacketType packetType)
{
    switch (packetType)
    {
    case PacketType::Undefined:
    case PacketType::Disconnection:
        return DeviceData::DeviceStatus::Undefined;

    case PacketType::Registration:
    case PacketType::Heartbeat:
        return DeviceData::DeviceStatus::Online;

    default:
        break;
    }

    return DeviceData::DeviceStatus::Undefined;
}

std::optional<DeviceData> DAMNListener::packed2DeviceData(const Packet& packet)
{
    DeviceData dd;
    dd.set_current_user(QString::fromStdString(packet.userId));
    dd.set_identifier(QString::fromStdString(packet.sourceId));
    dd.set_status(packetType2DeviceStatus(packet.type));
    dd.set_name(QString::fromStdString(packet.sourceId));
    return std::make_optional(dd);
}

} // namespace damn
