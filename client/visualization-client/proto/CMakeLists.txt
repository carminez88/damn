cmake_minimum_required(VERSION 3.14)

include(FindProtobuf)

find_package(Protobuf REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS packet.proto)
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS EXPORT_MACRO DLL_EXPORT packet.proto)
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS DESCRIPTORS PROTO_DESCS packet.proto)

message(STATUS "Proto header: ${PROTO_HDRS}")
message(STATUS "Proto sources: ${PROTO_SRCS}")
message(STATUS "Proto include dirs: ${PROTOBUF_INCLUDE_DIR}")
message(STATUS "Proto libs: ${protobuf_LIBRARIES}")
message(STATUS "Proto defs: ${protobuf_DEFINITIONS}")

add_library(proto ${PROTO_HDRS} ${PROTO_SRCS})
target_compile_definitions(proto PRIVATE ${protobuf_DEFINITIONS})
target_link_libraries(proto PRIVATE ${protobuf_LIBRARIES})