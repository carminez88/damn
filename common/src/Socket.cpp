#include <spdlog/spdlog.h>
#include "CerealUtils.h"
#include "Socket.h"

namespace damn {

DAMNSocket::DAMNSocket(std::string address)
    : m_address { std::move(address) }
{}

std::optional<Packet> DAMNSocket::read()
{
	try {

		zmq::message_t zmsg;

		if ( auto ret = m_zsocket->recv( zmsg ); ret ) {

            auto pkt = unpack<Packet>(std::string(static_cast<char*>(zmsg.data()), zmsg.size()));

            if ( pkt )
                return std::move( pkt.value() );

            spdlog::error( "Error while deserializing 0MQ message!" );
		}

    } catch (const std::exception& e) {
        spdlog::error( "Error while reading from 0MQ socket: {}", e.what() );
	}

    return {};
}

bool DAMNSocket::write(Packet& packet)
{
    if (auto buffer = pack<Packet>(packet); buffer.has_value()) {
        spdlog::error( "Cannot serialize message!" );
        return false;
    } else 
        return write( buffer.value() );

    return false;
}

bool DAMNSocket::write(const std::string& buffer)
{
    const auto dataLength = buffer.length();

    zmq::message_t zmsg( dataLength );

    std::memcpy( zmsg.data(), buffer.data(), dataLength );
    auto sendRet = m_zsocket->send( std::move( zmsg ), zmq::send_flags::dontwait );

    return sendRet.value_or( 0 ) == buffer.size();
}

} // namespace damn
