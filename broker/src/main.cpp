#include <CLI/App.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>
#include "Broker.h"

std::optional<std::string> parseIPAddress(int32_t argc, char *argv[]);

int32_t main(int32_t argc, char *argv[])
{    
    auto ipAddress = parseIPAddress( argc, argv );

    if ( not ipAddress.has_value() ) {
        return -1;
    }

    zmq::context_t ctx { 1 };

    damn::Broker::waitedRunBroker( ipAddress.value(), ctx );

    return 0;
}

std::optional<std::string> parseIPAddress(int32_t argc, char *argv[])
{
    std::string ipAddress;

    try {

        CLI::App app{ "DAMN Broker" };
        app.add_option("-i,--ip", ipAddress, "IPv4 Address");
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
