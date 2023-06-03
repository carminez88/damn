#include <thread>
#include <chrono>
#include "Publisher.h"
using namespace std::chrono_literals;

namespace damn {

DAMNPublisher::DAMNPublisher(std::string id, zmq::context_t& context)
    : ZMQLooper { context         }
    , m_id      { std::move( id ) }
{
    m_currentStatus.setUp( m_id );
}

void DAMNPublisher::onRequest(RequestData request)
{
    m_requests.emplace( request );
}

bool DAMNPublisher::createSocket()
{
    // FIXME: hardcoded address
    return createSocketImpl<ConnectInitializer<TcpAddressFormatter>>( net_data_t{ "127.0.0.1", 5555 }, zmq::socket_type::pub );
}

void DAMNPublisher::loopTask()
{
    processRequest();

    sendCurrentStatus();

    std::this_thread::sleep_for( 1s );
}

void DAMNPublisher::writePacket(Packet& packet)
{
    if ( not m_socket->write( packet ) )
        spdlog::error("Cannot write packet {}", packet.toString() );
    else
        spdlog::info("Sent packet {}", packet.toString());
}

void DAMNPublisher::sendCurrentStatus()
{
    Packet pkt = m_currentStatus.toPacket();
    writePacket( pkt );
}

void DAMNPublisher::disconnect()
{
    m_currentStatus.packetType = PacketType::Disconnection;
    m_currentStatus.userId = "";
    m_currentStatus.activityDetails = "";
    m_currentStatus.requestType = RequestType::Undefined;

    sendCurrentStatus();
}

void DAMNPublisher::processRequest()
{
    if ( not m_requests.empty() ) {

        RequestData request = m_requests.front();

        m_currentStatus.loadFrom( request );

        m_requests.pop();
    }
}

void DAMNPublisher::Status::setUp(std::string identifier)
{
    id = std::move( identifier );
    packetType = PacketType::Heartbeat;
    userId = "";
    activityDetails = "";
    requestType = RequestType::Undefined;
}

void DAMNPublisher::Status::loadFrom(const RequestData &request)
{
    switch ( request.requestType ) {
    case RequestType::Registration:
    {
        userId = request.userId;
        activityDetails = request.activityDetails;
        requestType = request.requestType;
        packetType = PacketType::Registration;
    }
    break;

    case RequestType::Undefined:
    case RequestType::Disconnection:
        setUp( id );
        break;
    }
}

Packet DAMNPublisher::Status::toPacket()
{
    Packet pkt;
    pkt.sourceId = id;
    pkt.timestamp = std::time( nullptr );
    pkt.type = packetType;
    pkt.userId = userId;
    pkt.details = activityDetails;

    return pkt;
}


} // namespace damn
