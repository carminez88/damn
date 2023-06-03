#pragma once

#include <spdlog/spdlog.h>
#include "Packet.h"
#include "ZMQUtils.h"

namespace damn {

class DAMNSocket
{
public:
    explicit DAMNSocket(net_data_t netData);

    DAMNSocket(const DAMNSocket&) = delete;
    DAMNSocket& operator=(const DAMNSocket&) = delete;

    template <typename InitializerPolicy>
    [[nodiscard]] bool init(zmq_context_holder_t context, zmq_socket_type_t type)
    {
        try {

            m_zsocket = makeSocket<InitializerPolicy>( context.get(), type, m_netData );

            return true;

        } catch (const std::exception& e) {
            spdlog::error("Error while creating 0MQ socket: {}", e.what());
        }

        return false;
    }

    [[nodiscard]] std::optional<Packet> read();

    [[nodiscard]] bool write(Packet& packet);

    [[nodiscard]] bool write(const std::string& buffer);

    // TODO: socket option setter

private:
    net_data_t m_netData;
    zmq_socket_type_t m_zsocketType{ zmq_socket_type_t::req };
    zmq_socket_t m_zsocket;
    const int32_t k_timeout = 10000; // NOTE: timeout a 10 [s]
};

} // namespace damn
