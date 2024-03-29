#include "Broker.h"

namespace damn {

Broker::Broker(zmq_context_t &ctx)
    : m_ctx { ctx }
{}

void Broker::run()
{
    spdlog::info( "Starting broker..." );

    constexpr int32_t k_frontendPort { 5555 };
    constexpr int32_t k_backendPort { 5556 };

    try {
        auto frontend = makeSocket<BindInitializer<TcpAddressFormatter>>( m_ctx, zmq::socket_type::xsub, { "*", k_frontendPort } );
        auto backend = makeSocket<BindInitializer<TcpAddressFormatter>>( m_ctx, zmq::socket_type::xpub, { "*", k_backendPort } );
        auto control = makeSocket<ConnectInitializer<InprocAddressFormatter>>( m_ctx, zmq::socket_type::sub, { "control" } );
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

auto Broker::runBroker(zmq_context_t &ctx)
{
    Broker broker { ctx };
    return runBroker( broker );
}

void Broker::waitedRunBroker(zmq_context_t &ctx)
{
    Broker broker { ctx };
    waitedRunBroker( broker );
}




} // namespace damn
