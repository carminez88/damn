#include "Broker.h"

namespace damn {

Broker::Broker(std::string ipAddress, zmq_context_t &ctx)
    : m_ctx ( ctx )
    , m_ipAddress ( std::move( ipAddress ) )
{}

void Broker::run()
{
    spdlog::info( "Starting broker, using IP address {}", m_ipAddress );

    constexpr int32_t k_frontendPort { 5555 };
    constexpr int32_t k_backendPort { 5556 };

    try {
        auto frontend = makeSocket<BindInitializer<TcpAddressFormatter>>( m_ctx, zmq::socket_type::xsub, m_ipAddress, k_frontendPort );
        auto backend = makeSocket<BindInitializer<TcpAddressFormatter>>( m_ctx, zmq::socket_type::xpub, m_ipAddress, k_backendPort );
        auto control = makeSocket<ConnectInitializer<InprocAddressFormatter>>( m_ctx, zmq::socket_type::sub, "control" );
        control.set( zmq::sockopt::subscribe, "" );

        spdlog::info( "Broker started!" );

        zmq::proxy_steerable( zmq_socket_ref_t{ frontend }, zmq_socket_ref_t{ backend }, {}, zmq_socket_ref_t{ control } );

    }  catch (std::exception& e) {
        spdlog::error( "ZMQ error while starting broker: {}", e.what() );
        return;
    }

    spdlog::info( "Broker stopped!" );
}

auto Broker::runBroker(Broker &broker)
{
    return std::async( std::launch::async, &Broker::run, broker );
}

void Broker::waitedRunBroker(Broker &broker)
{
    runBroker( broker ).wait();
}

auto Broker::runBroker(std::string ipAddress, zmq_context_t &ctx)
{
    Broker broker { ipAddress, ctx };
    return runBroker( broker );
}

void Broker::waitedRunBroker(std::string ipAddress, zmq_context_t &ctx)
{
    Broker broker { ipAddress, ctx };
    waitedRunBroker( broker );
}




} // namespace damn
