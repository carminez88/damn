#include <spdlog/spdlog.h>
#include "Socket.h"

namespace damn {

DAMNSocket::DAMNSocket(std::string address)
    : m_address { std::move(address) }
{}

std::optional<Packet> DAMNSocket::read()
{
	try {

		zmq::message_t zmsg;

		if ( auto ret = m_zsocket->recv( zmsg ); ret.has_value() ) {

            Packet pkt;
            const bool parseSuccessfull = pkt.ParseFromString( std::string( static_cast<char*>( zmsg.data() ), zmsg.size() ) );

            if ( parseSuccessfull )
                return std::move( pkt );

            spdlog::error( "Error while deserializing 0MQ message!" );
		}

    } catch (const std::exception& e) {
        spdlog::error( "Error while reading from 0MQ socket: {}", e.what() );
	}

    return {};
}

bool DAMNSocket::write(Packet& packet)
{
    std::string buffer;

    if ( not packet.SerializeToString( &buffer ) ) {
        spdlog::error( "Cannot serialize message!" );
        return false;
    }

    return write( buffer );
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
