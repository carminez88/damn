#pragma once

#include <QObject>
#include <qobjectdefs.h>

#include "DeviceData.h"
#include "ZMQUtils.h"
#include "IStoppableRunner.h"
#include "Packet.h"
#include "Socket.h" // TODO: pimplare

namespace damn {

class DAMNListener : public QObject, 
                     public IStoppableRunner
{
    Q_OBJECT

    using socket_t = DAMNSocket;
    using socket_ptr_t = std::unique_ptr<socket_t>;
public:
    explicit DAMNListener(zmq::context_t& context);

    void run(std::stop_token stoken) override;

signals:
    void notifyPacket(Packet);
    void notifyDevice(DeviceData);

private:
    [[nodiscard]] static std::optional<DeviceData> packed2DeviceData(const Packet& packet);

private:
    zmq_context_holder_t m_context;
    socket_ptr_t m_socket { nullptr };
};

} // namespace damn
