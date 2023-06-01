#pragma once

#include <QObject>
#include <unordered_map>
#include <queue>
#include "RequestData.h"
#include "ZMQUtils.h"
#include "IStoppableRunner.h"
#include "Packet.h"
#include "Socket.h" // TODO: pimplare

namespace damn {

class DAMNPublisher : public QObject,
                      public IStoppableRunner
{
    Q_OBJECT

    using socket_t = DAMNSocket;
    using socket_ptr_t = std::unique_ptr<socket_t>;
    using queue_t = std::queue<RequestData>;

    struct Status : public RequestData
    {
        std::string id;
        PacketType packetType;

        void setUp(std::string identifier);

        void loadFrom(const RequestData& request);

        [[nodiscard]] Packet toPacket();
    };

public:
    explicit DAMNPublisher(std::string id, zmq::context_t& context);

    void run(std::stop_token stoken) override;

    void onRequest(RequestData request);

private:
    void writePacket(Packet& packet);

    void sendCurrentStatus();

    void disconnect();

    void processRequest();

private:
    std::string m_id;
    zmq_context_holder_t m_context;
    socket_ptr_t m_socket { nullptr };
    queue_t m_requests;
    Status m_currentStatus;
};

} // namespace damn
