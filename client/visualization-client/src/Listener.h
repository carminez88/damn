#ifndef DAMNLISTENER_H
#define DAMNLISTENER_H

#include <qobjectdefs.h>

#include "Packet.h"
#include "Socket.h" // TODO: pimplare

namespace damn {

class DAMNListener
{
	using socket_t = DAMNSocket;
	using socket_ptr_t = std::unique_ptr<socket_t>;
	using zmq_context_t = std::reference_wrapper<zmq::context_t>;
public:
	explicit DAMNListener(zmq::context_t& context);

	void run();

Q_SIGNALS:
	void notifyPacket(Packet);

public Q_SLOTS:
	void on_updateRunningStatus(bool a_isRunning);
	
private:
	bool m_isRunning { true };
	zmq_context_t m_context;
	socket_ptr_t m_socket { nullptr };
};

} // namespace damn

#endif // DAMNLISTENER_H