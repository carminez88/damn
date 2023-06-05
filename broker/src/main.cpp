#include "Broker.h"

std::optional<std::string> parseIPAddress(int32_t argc, char *argv[]);

int32_t main(int32_t argc, char *argv[])
{
    zmq::context_t ctx { 1 };

    damn::Broker::waitedRunBroker( ctx );

    return 0;
}
