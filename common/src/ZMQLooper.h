#pragma once

#include "Looper.h"
#include "Socket.h"
#include "ZMQUtils.h"

namespace damn {

class ZMQLooper : public Looper
{
public:
    ZMQLooper(std::string address, zmq_context_t& ctx);

    virtual ~ZMQLooper() override = default;

protected:
    void beforeLoop() override;

    [[nodiscard]] virtual bool createSocket() = 0;

    template<typename InitPolicy>
    [[nodiscard]] bool createSocketImpl(net_data_t netData, zmq_socket_type_t type)
    {
        m_socket = std::make_unique<socket_t>( netData );

        if ( not m_socket->init<InitPolicy>( m_context, type ) ) {
            spdlog::error( "Failed to initialize socket!" );
            return false;
        }

        spdlog::info( "Socket initialized!" );

        return true;
    }

    virtual void loopTask() override = 0;

protected:
    using socket_t = DAMNSocket;
    using socket_ptr_t = std::unique_ptr<socket_t>;

    zmq_context_holder_t m_context;
    socket_ptr_t m_socket { nullptr };
    std::string m_address;
};

}
