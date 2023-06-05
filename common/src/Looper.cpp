#include <spdlog/spdlog.h>
#include "Looper.h"

namespace damn {

void Looper::run(std::stop_token stoken)
{
    try {
        beforeLoop();
    }  catch (std::exception& e) {
        spdlog::error( "Received error before loop: {}.", e.what() );
        return;
    }

    while (true) {

        if ( stoken.stop_requested() ) {
            spdlog::info( "Stop requested, so I'm closing..." );
            return;
        }

        loopTask();

    }

    afterLoop();
}

void Looper::beforeLoop()
{
    // dummy
}

void Looper::afterLoop()
{
    // dummy
}

} // namespace damn
