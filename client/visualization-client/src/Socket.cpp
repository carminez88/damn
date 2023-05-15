#include <spdlog/spdlog.h>
#include "Socket.h"

namespace damn {

DAMNSocket::DAMNSocket(std::string address)
    : m_address ( std::move( address ) )
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

	return std::nullopt;
}

} // namespace damn
