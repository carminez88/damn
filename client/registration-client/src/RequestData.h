#pragma once

#include <string>
#include "Packet.h"

namespace damn {

enum class RequestType
{
    Undefined    = -1,
    Registration = 0,
    Disconnection
};

struct RequestData
{
    std::string userId;
    std::string activityDetails;
    RequestType requestType;
};

} // namespace damn
