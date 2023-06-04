#pragma once

#include <thread>
#include <future>
#include <spdlog/spdlog.h>
#include "ZMQUtils.h"

namespace damn {

class Broker
{
public:
    explicit Broker(zmq_context_t& ctx);

    void run();

    [[nodiscard]] static auto runBroker(Broker& broker);

    static void waitedRunBroker(Broker& broker);

    [[nodiscard]] static auto runBroker(zmq_context_t& ctx);

    static void waitedRunBroker(zmq_context_t& ctx);

private:
    zmq_context_holder_t m_ctx;
};

} // namespace damn
