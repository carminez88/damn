#pragma once

#include "Socket.h" 
#include "IStoppableRunner.h"

namespace damn {

class DummyDevicePublisher : public IStoppableRunner
{
	using socket_t = DAMNSocket;
	using socket_ptr_t = std::unique_ptr<socket_t>;
public:
	struct DummyDetails
	{
		std::string user;
		std::string source;
		std::string details;
	};

	DummyDevicePublisher(DummyDetails details, zmq::context_t& context);

	void run(std::stop_token stoken) override;

private:
	zmq_context_holder_t m_context;
	socket_ptr_t m_socket{ nullptr };
	DummyDetails m_details;
};

} // namespace damn