#include <QCoreApplication>
#include <spdlog/spdlog.h>
#include "Listener.h"

namespace damn 
{

bool DAMNListener::createSocket()
{
    constexpr int32_t k_connectionPort { 5556 };

    if ( createSocketImpl<ConnectInitializer<TcpAddressFormatter>>( net_data_t{ m_address, k_connectionPort }, zmq::socket_type::sub ) ) {
        m_socket->setOption( zmq::sockopt::rcvtimeo, k_timeout );
        m_socket->setOption( zmq::sockopt::subscribe, "" );
        return true;
    }

    return false;
}

void DAMNListener::loopTask()
{
    // Listen on ZMQ socket with a timeout of X seconds
    // If package is found, emit it
    //spdlog::info("Shhhh...I'm listening...");

    if ( auto pktRet = m_socket->read(); pktRet ) {

        auto packet = std::move( pktRet.value() );

        //spdlog::debug( "Received packet {}", packet.toString() );

        if ( const auto dd = packed2DeviceData(packet); dd ) {
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
