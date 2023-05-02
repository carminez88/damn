#ifndef SOCKET_H
#define SOCKET_H

#include <zmq.hpp>

#include "Packet.h"

namespace damn {

	template<typename T>
	struct RefHolder
	{
		RefHolder(T& type)
			: toHold( type )
		{}

		T& operator()() 
		{
			return toHold;
		}

		T& toHold;
	};

	class DAMNSocket
	{
		using zmq_socket_t = zmq::socket_t;
		using zmq_socket_ptr_t = std::unique_ptr<zmq_socket_t>;
		using zmq_socket_type_t = zmq::socket_type;
		using zmq_context_t = zmq::context_t;
		using zmq_context_holder_t = RefHolder<zmq_context_t>;
	public:
		// TODO: Move ctor and otor

		DAMNSocket(const DAMNSocket&) = delete;
		DAMNSocket& operator=(const DAMNSocket&) = delete;

		[[nodiscard]] bool init(zmq_context_holder_t& context, zmq_socket_type_t type);

		[[nodiscard]] std::optional<Packet> read();

	private:
		std::string m_address { "" };
		zmq_socket_type_t m_zsocketType{ zmq_socket_type_t::req };
		zmq_socket_ptr_t m_zsocket { nullptr };
	};

} // namespace damn

#endif // SOCKET_H