#ifndef SOCKET_H
#define SOCKET_H

#include <spdlog/spdlog.h>
#include "Packet.h"
#include "ZMQUtils.h"

namespace damn {

struct BindPolicy
{
    inline static void apply(zmq_socket_t& socket, std::string_view address)
    {
        socket.bind( address.data() );
    }
};

struct ConnectPolicy
{
    inline static void apply(zmq_socket_t& socket, std::string_view address)
    {
        socket.connect( address.data() );
    }
};

class DAMNSocket
{
public:
    explicit DAMNSocket(std::string address);

    DAMNSocket(const DAMNSocket&) = delete;
    DAMNSocket& operator=(const DAMNSocket&) = delete;

    template <typename InitPolicy>
    [[nodiscard]] bool init(zmq_context_holder_t context, zmq_socket_type_t type)
    {
        constexpr int32_t k_timeout{ 10000 }; // NOTE: timeout a 10 [s]

        try {

            m_zsocket = std::make_unique<zmq_socket_t>(context.get(), type);
            m_zsocket->set(zmq::sockopt::rcvtimeo, k_timeout);

            InitPolicy::apply( *m_zsocket.get(), m_address );

            return true;

        }
        catch (const std::exception& e) {
            spdlog::error("Error while creating 0MQ socket: {}", e.what());
        }

        return false;
    }

    [[nodiscard]] std::optional<Packet> read();

private:
    std::string m_address { "" };
    zmq_socket_type_t m_zsocketType{ zmq_socket_type_t::req };
    zmq_socket_ptr_t m_zsocket { nullptr };
};

} // namespace damn

#endif // SOCKET_H
