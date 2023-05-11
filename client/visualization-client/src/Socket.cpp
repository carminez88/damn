//#include "spdlog/spdlog.h"
#include "Socket.h"

namespace damn {

	bool DAMNSocket::init(zmq_context_holder_t context, zmq_socket_type_t type)
	{
        constexpr int32_t k_timeout { 10000 }; // NOTE: timeout a 10 [s]

		try{

			m_zsocket = std::make_unique<zmq_socket_t>( context.get(), type );
            m_zsocket->set( zmq::sockopt::rcvtimeo, k_timeout );
            // TODO: bind or connect
            return true;

        } catch (const std::exception& e) {
            //spdlog::error( "Error while creating 0MQ socket: {}", e.what() );
		}

        return false;
	}

	std::optional<Packet> DAMNSocket::read()
	{
		try {

			zmq::message_t zmsg;

			if ( auto ret = m_zsocket->recv( zmsg ); ret.has_value() ) {

                Packet pkt;
                const bool parseSuccessfull = pkt.ParseFromString( std::string( static_cast<char*>( zmsg.data() ), zmsg.size() ) );

                if ( parseSuccessfull )
                    return std::move( pkt );

                //spdlog::error( "Error while deserializing 0MQ message!" );
			}

        } catch (const std::exception& e) {
            //spdlog::error( "Error while reading from 0MQ socket: {}", e.what() );
		}

		return std::nullopt;
	}

} // namespace damn
