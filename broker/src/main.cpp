#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <spdlog/spdlog.h>
#include <CLI/App.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>

zmq::socket_t makeSocket(zmq::context_t& ctx, zmq::socket_type type, std::string ipAddress, int32_t port);

std::optional<std::string> parseIPAddress(int32_t argc, char *argv[]);

int32_t main(int32_t argc, char *argv[])
{
    auto ipAddress = parseIPAddress( argc, argv );

    if ( not ipAddress.has_value() ) {
        return -1;
    }

    spdlog::info( "Using IP address {}", ipAddress.value() );

    zmq::context_t context { 1 };

    constexpr int32_t k_frontendPort { 5555 };
    constexpr int32_t k_backendPort { 5556 };
    zmq::socket_t frontend = makeSocket( context, zmq::socket_type::xsub, ipAddress.value(), k_frontendPort );
    zmq::socket_t backend = makeSocket( context, zmq::socket_type::xpub, ipAddress.value(), k_backendPort );

    spdlog::info( "Broker started!", ipAddress.value() );

    zmq::proxy( zmq::socket_ref{ frontend, }, zmq::socket_ref{ backend }, {} );

    return 0;
}

zmq::socket_t makeSocket(zmq::context_t& ctx, zmq::socket_type type, std::string ipAddress, int32_t port)
{
    zmq::socket_t socket { ctx, type };

    const std::string zmqAddress = fmt::format( "tcp://{}:{}", ipAddress, port );
    socket.bind( zmqAddress );

    return std::move( socket );
}

std::optional<std::string> parseIPAddress(int32_t argc, char *argv[])
{
    CLI::App app{ "DAMN Broker" };

    std::string ipAddress;
    app.add_option("-i,--ip", ipAddress, "IPv4 Address");

    try {
        app.parse(argc, argv);
    } catch(const CLI::ParseError &e) {
        spdlog::info( "Error while parsing IP Address: {}", e.what() );
        return std::nullopt;
    }

    if ( ipAddress.empty() ) {
        spdlog::error( "Unable to read -i (--ip) option. Please run the application with -i (--ip) <IPv4 Address> option!" );
        return std::nullopt;
    }

    return std::make_optional( std::move( ipAddress ) );
}
