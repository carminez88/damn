#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <spdlog/spdlog.h>
#include <CLI/App.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>

int32_t main(int32_t argc, char *argv[])
{
    CLI::App app{ "DAMN Broker" };

    std::string ipAddress;
    app.add_option("-i,--ip", ipAddress, "IPv4 Address");

    CLI11_PARSE( app, argc, argv );

    if ( ipAddress.empty() ) {
        spdlog::error( "Unable to read -i (--ip) option. Please run the application with -i (--ip) <IPv4 Address> option!" );
        return -1;
    }

    spdlog::info( "Using IP address {}", ipAddress );

    zmq::context_t context { 1 };

    zmq::socket_t frontend { context, zmq::socket_type::xsub };
    frontend.bind( "tcp://127.0.0.1:5555" );

    zmq::socket_t backend { context, zmq::socket_type::xpub };
    backend.bind( "tcp://127.0.0.1:5556" );

    zmq::proxy( zmq::socket_ref{ frontend, }, zmq::socket_ref{ backend }, {} );

    return 0;
}
