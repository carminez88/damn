cmake_minimum_required(VERSION 3.20)

include(FindProtobuf)

include( ../../cmake/DAMNFindProtobuf.cmake )

include_directories(${PROTOBUF_INCLUDE_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS packet.proto)
file(COPY ${PROTO_HDRS} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
file(COPY ${PROTO_SRCS} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS EXPORT_MACRO DLL_EXPORT packet.proto)
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS DESCRIPTORS PROTO_DESCS packet.proto)

message(STATUS "Proto header: ${PROTO_HDRS}")
message(STATUS "Proto sources: ${PROTO_SRCS}")
message(STATUS "Proto include dirs: ${PROTOBUF_INCLUDE_DIR}")
message(STATUS "Proto libs: ${protobuf_LIBRARIES}")
message(STATUS "Proto defs: ${protobuf_DEFINITIONS}")

set(TARGET_NAME "proto")

add_library( ${TARGET_NAME} STATIC ${PROTO_HDRS} ${PROTO_SRCS} )
add_library( DAMN::Proto ALIAS ${TARGET_NAME}                  )

target_include_directories(${TARGET_NAME} PRIVATE ${PROTOBUF_INCLUDE_DIR}  )

target_compile_definitions(${TARGET_NAME} PRIVATE ${protobuf_DEFINITIONS})

target_link_libraries(${TARGET_NAME} PRIVATE ${protobuf_LIBRARIES})