#pragma once

#include "../Socket.h" 

namespace damn {

class DummyDevicePublisher
{
	using socket_t = DAMNSocket;
	using socket_ptr_t = std::unique_ptr<socket_t>;

public:
	DummyDevicePublisher(zmq::context_t& context);

	void run();

private:
	zmq_context_holder_t m_context;
	socket_ptr_t m_socket{ nullptr };
};

} // namespace damn