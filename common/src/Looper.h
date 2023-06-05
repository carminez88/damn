#pragma once

#include "IStoppableRunner.h"

namespace damn {

class Looper : public IStoppableRunner
{
public:
    virtual ~Looper() = default;

    void run(std::stop_token stoken) override;

protected:
    virtual void beforeLoop();

    virtual void loopTask() = 0;

    virtual void afterLoop();
};

} // namespace damn
