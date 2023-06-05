#pragma once

#include <string>
#include <optional>

namespace damn {

struct NetData
{
    std::string address;
    std::optional<int32_t> port {};
};

using net_data_t = NetData;

} // namespace damn
