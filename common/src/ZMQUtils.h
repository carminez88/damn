#ifndef ZMQUTILS_H
#define ZMQUTILS_H

#include <zmq.hpp>

namespace damn {

using zmq_socket_t = zmq::socket_t;
using zmq_socket_ptr_t = std::unique_ptr<zmq_socket_t>;
using zmq_socket_type_t = zmq::socket_type;
using zmq_context_t = zmq::context_t;
using zmq_context_holder_t = std::reference_wrapper<zmq_context_t>;

} // namespace damn

#endif // ZMQUTILS_H
