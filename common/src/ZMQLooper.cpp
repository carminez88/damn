#include <spdlog/spdlog.h>
#include "ZMQLooper.h"

namespace damn {

ZMQLooper::ZMQLooper(zmq_context_t& ctx)
    : m_context { ctx }
{
}

void ZMQLooper::beforeLoop()
{
    if ( not createSocket() )
        throw std::runtime_error( "Error while creating socket!" );
}

}

