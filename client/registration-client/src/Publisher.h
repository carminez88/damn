#pragma once

#include <QObject>
#include <queue>
#include "DeviceData.h"
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
    using queue_t = std::queue<DeviceData>;
public:
    explicit DAMNPublisher(zmq::context_t& context);

    void run(std::stop_token stoken) override;

    void onRegistrationRequest(DeviceData deviceData);

private:
    [[nodiscard]] static Packet deviceData2Packet(const DeviceData& deviceData);
private:
    zmq_context_holder_t m_context;
    socket_ptr_t m_socket { nullptr };
    queue_t m_requests;
};

} // namespace damn
