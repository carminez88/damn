#ifndef SOCKET_H
#define SOCKET_H

#include "Packet.h"
#include "ZMQUtils.h"

namespace damn {

class DAMNSocket
{

public:
    DAMNSocket() = default;

    DAMNSocket(const DAMNSocket&) = delete;
    DAMNSocket& operator=(const DAMNSocket&) = delete;

    [[nodiscard]] bool init(zmq_context_holder_t context, zmq_socket_type_t type);

    [[nodiscard]] std::optional<Packet> read();

private:
    std::string m_address { "" };
    zmq_socket_type_t m_zsocketType{ zmq_socket_type_t::req };
    zmq_socket_ptr_t m_zsocket { nullptr };
};

} // namespace damn

#endif // SOCKET_H
