#pragma once

#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <spdlog/spdlog.h>
#include "NetData.h"

namespace damn {

using zmq_socket_t = zmq::socket_t;
using zmq_socket_ptr_t = std::unique_ptr<zmq_socket_t>;
using zmq_socket_ref_t = zmq::socket_ref;
using zmq_socket_type_t = zmq::socket_type;
using zmq_context_t = zmq::context_t;
using zmq_context_holder_t = std::reference_wrapper<zmq_context_t>;

inline static std::string message2string(zmq::message_t& msg)
{
    return std::string( static_cast<char*>( msg.data() ), msg.size() );
}

inline static zmq::message_t string2message(const std::string& string)
{
    const auto dataLength = string.length();

    zmq::message_t zmsg( dataLength );

    std::memcpy( zmsg.data(), string.data(), dataLength );

    return zmsg;
}

struct SocketOperator
{
    virtual zmq_socket_t apply() = 0;
};

class SocketBaseCreator : public SocketOperator
{
public:
    SocketBaseCreator(zmq_context_t& ctx, zmq_socket_type_t type)
        : m_ctx   { ctx  }
        , m_sType { type }
    {}

    zmq_socket_t apply() override
    {
        return zmq_socket_t{ m_ctx, m_sType };
    }

private:
    zmq_context_holder_t m_ctx;
    zmq_socket_type_t m_sType;
};

class SocketInitializer : public SocketOperator
{
public:
    SocketInitializer(std::string_view address, std::optional<int32_t> port, SocketOperator& op)
        : m_operator { op                }
        , m_address  { address           }
        , m_port     { std::move( port ) }
    {
    }

    virtual zmq_socket_t apply() override = 0;

protected:
    SocketOperator& m_operator;
    std::string m_address;
    std::optional<int32_t> m_port;
};

struct TcpAddressFormatter
{
    static std::string format(std::string_view address, int32_t port)
    {
        return fmt::format( "tcp://{}:{}", address, port );
    }
};

struct InprocAddressFormatter
{
    static std::string format(std::string_view address, [[maybe_unused]]int32_t port)
    {
        return fmt::format( "inproc://{}", address );
    }
};

template<typename FormatPolicy>
class ConnectInitializer : public SocketInitializer
{
public:
    using SocketInitializer::SocketInitializer;

    zmq_socket_t apply() override
    {
        auto socket = m_operator.apply();
        auto address = FormatPolicy::format( m_address, m_port.value_or( 0 ) );

        try {
            socket.connect( address );
        }  catch (std::exception& e) {
            spdlog::error( "Error while connecting at address {}: {}", address, e.what() );
        }

        return std::move( socket );
    }
};

template<typename FormatPolicy>
class BindInitializer : public SocketInitializer
{
public:
    using SocketInitializer::SocketInitializer;

    zmq_socket_t apply() override
    {
        auto socket = m_operator.apply();
        auto address = FormatPolicy::format( m_address, m_port.value_or( 0 ) );

        try {
            socket.bind( address );
        }  catch (std::exception& e) {
            spdlog::error( "Error while binding at address {}: {}", address, e.what() );
        }

        return std::move( socket );
    }
};

template<typename InitializerPolicy>
auto makeSocket(zmq_context_t& ctx, zmq_socket_type_t type, const net_data_t& netData)
{
    SocketBaseCreator baseCreator { ctx, type };

    InitializerPolicy initializer { netData.address, netData.port, baseCreator };

    // TODO: find a way to set options

    return initializer.apply();
}

} // namespace damn
