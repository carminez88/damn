#pragma once

#include <QObject>

#include "DeviceData.h"
#include "Packet.h"
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

    const int32_t k_timeout = 10000; // NOTE: timeout a 10 [s]
};

} // namespace damn
