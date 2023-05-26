#pragma once

#include <thread>
#include <future>
#include <spdlog/spdlog.h>
#include "ZMQUtils.h"

namespace damn {

class Broker
{
public:
    explicit Broker(std::string ipAddress, zmq_context_t& ctx);

    void run();

    static auto runBroker(Broker& broker);

    static void waitedRunBroker(Broker& broker);

    static auto runBroker(std::string ipAddress, zmq_context_t& ctx);

    static void waitedRunBroker(std::string ipAddress, zmq_context_t& ctx);

private:
    zmq_context_holder_t m_ctx;
    std::string m_ipAddress;
};

} // namespace damn
