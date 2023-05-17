#pragma once

#include <thread>

namespace damn {

class IStoppableRunner
{
public:
    virtual ~IStoppableRunner() = default;
    virtual void run(std::stop_token stoken) = 0;
};

}