#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <spdlog/spdlog.h>

int32_t main(int32_t argc, char *argv[])
{
    zmq::context_t context { 1 };

    zmq::socket_t frontend { context, zmq::socket_type::xsub };
    frontend.bind( "tcp://127.0.0.1:5555" );

    zmq::socket_t backend { context, zmq::socket_type::xpub };
    backend.bind( "tcp://127.0.0.1:5556" );

    zmq::proxy( zmq::socket_ref{ frontend, }, zmq::socket_ref{ backend }, {} );

    return 0;
}
