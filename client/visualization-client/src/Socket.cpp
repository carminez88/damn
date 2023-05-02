#include "Socket.h"

namespace damn {

	bool DAMNSocket::init(zmq_context_holder_t context, zmq_socket_type_t type)
	{
		try{
			m_zsocket = std::make_unique<zmq_socket_t>( context.get(), type );
			m_zsocket->set( zmq::sockopt::rcvtimeo, 10000 ); // NOTE: timeout a 10 [s]
			// TODO: bind or connect
		} catch (const std::exception&) {
			// TODO: log!
			return false;
		}

		return true;
	}

	std::optional<Packet> DAMNSocket::read()
	{
		try {

			zmq::message_t zmsg;

			if ( auto ret = m_zsocket->recv( zmsg ); ret.has_value() ) {
				// TODO: handle ZMQ message and convert it to a Packet
				Packet pkt;

				return std::move( pkt );
			}

		} catch (const std::exception&) {
			// TODO: log!
		}
		return std::nullopt;
	}

} // namespace damn