#pragma once

#include <QObject>
#include <unordered_map>
#include <queue>
#include "Packet.h"
#include "RequestData.h"
#include "ZMQLooper.h"

namespace damn {

class DAMNPublisher : public QObject,
                      public ZMQLooper
{
    Q_OBJECT
public:
    explicit DAMNPublisher(std::string id, std::string address, zmq::context_t& context);

    void onRequest(RequestData request);

private:
    [[nodiscard]] bool createSocket() override;

    void loopTask() override;

    void writePacket(Packet& packet);

    void sendCurrentStatus();

    void disconnect();

    void processRequest();

private:
    using queue_t = std::queue<RequestData>;

    struct Status : public RequestData
    {
        std::string id;
        PacketType packetType;

        void setUp(std::string identifier);

        void loadFrom(const RequestData& request);

        [[nodiscard]] Packet toPacket();
    };

    std::string m_id;
    queue_t m_requests;
    Status m_currentStatus;
};

} // namespace damn
