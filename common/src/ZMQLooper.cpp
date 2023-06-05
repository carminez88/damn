#include <spdlog/spdlog.h>
#include "ZMQLooper.h"

namespace damn {

ZMQLooper::ZMQLooper(std::string address, zmq_context_t& ctx)
    : m_address { std::move( address ) }
    , m_context { ctx                  }
{
}

void ZMQLooper::beforeLoop()
{
    if ( not createSocket() )
        throw std::runtime_error( "Error while creating socket!" );
}

}

