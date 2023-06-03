#pragma once

#include <QObject>

#include "DeviceData.h"
#include "Packet.h"
#include "Socket.h" // TODO: pimplare
#include "ZMQLooper.h"

namespace damn {

class DAMNListener : public QObject, 
                     public ZMQLooper
{
    Q_OBJECT
public:
    using ZMQLooper::ZMQLooper;

private:
    [[nodiscard]] bool createSocket() override;

    void loopTask() override;

signals:
    void notifyPacket(Packet);
    void notifyDevice(DeviceData);

private:
    [[nodiscard]] static std::optional<DeviceData> packed2DeviceData(const Packet& packet);
};

} // namespace damn
