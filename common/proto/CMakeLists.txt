cmake_minimum_required(VERSION 3.20)

include(FindProtobuf)

include( ../../cmake/DAMNFindProtobuf.cmake )

include_directories(${PROTOBUF_INCLUDE_DIR})
set(TARGET_NAME "proto")
add_library(${TARGET_NAME} STATIC)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS packet.proto)

message(STATUS "Proto header: ${PROTO_HDRS}")
message(STATUS "Proto sources: ${PROTO_SRCS}")
message(STATUS "Proto include dirs: ${PROTOBUF_INCLUDE_DIR}")
message(STATUS "Proto libs: ${Protobuf_LIBRARIES}")
message(STATUS "Proto defs: ${protobuf_DEFINITIONS}")
message(STATUS "Proto header include dir: ${PROTO_INCLUDE_DIR}")

target_sources(${TARGET_NAME}
  PUBLIC
  ${PROTO_HDRS}
  PRIVATE
  ${PROTO_SRCS}
)

# This is almost a hack, so let's spend a few words on it...
# In order to allow users of the proto library to include PROTO_HDRS,
# the containing folder must be added to the target include directory
# as a public interface.
#
# Note: The usage of the following line to acquire ${PROTO_INCLUDE_DIR} could
# be avoided by directly using CMAKE_CURRENT_BINARY_DIR, however ~ to a personal taste ~
# this solution seems more elegant
get_filename_component(PROTO_INCLUDE_DIR ${PROTO_HDRS} DIRECTORY)

target_include_directories(${TARGET_NAME}
  PRIVATE
  ${PROTOBUF_INCLUDE_DIR}
  PUBLIC
  ${PROTO_INCLUDE_DIR}
)

target_link_libraries(${TARGET_NAME}
  PRIVATE
  ${Protobuf_LIBRARIES}
)

add_library( DAMN::Proto ALIAS ${TARGET_NAME})
