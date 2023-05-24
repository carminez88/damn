cmake_minimum_required( VERSION 3.20 )

project( Common CXX )

include( ../cmake/DAMNCompileOptions.cmake )
include( ../cmake/DAMNCommonOptions.cmake  )
include( ../cmake/DAMNFindZeroMQ.cmake     )
include( ../cmake/DAMNFindCppZmq.cmake     )
include( ../cmake/DAMNFindSpdlog.cmake     )
include( ../cmake/DAMNFindFmt.cmake        )
include( ../cmake/DAMNFindProtobuf.cmake   )

add_subdirectory(proto)

include_directories(${Protobuf_INCLUDE_DIRS})

set(TARGET_NAME ${PROJECT_NAME})

file(GLOB HDRS "src/*.h")
file(GLOB SRCS "src/*.cpp")

add_library ( ${TARGET_NAME} STATIC ${HDRS} ${SRCS} )
add_library ( DAMN::Common ALIAS ${TARGET_NAME}     )

target_link_libraries(${TARGET_NAME} PRIVATE ${ZeroMQ_LIBRARIES} 
                                             ${spdlog_LIBRARIES}
                                             ${fmt_LIBRARIES}
                                             ${cppzmq_LIBRARIES}
                                             ${protobuf_LIBRARIES}
                                             DAMN::Proto)

target_compile_definitions(${TARGET_NAME} PRIVATE ${spdlog_DEFINITIONS} 
                                                  ${fmt_DEFINITIONS}
                                                  ${protobuf_DEFINITIONS}
                                                  ${ZeroMQ_DEFINITIONS}
                                                  ${cppzmq_DEFINITIONS})

include_directories(${CMAKE_CURRENT_BINARY_DIR}    )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}    )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

target_include_directories(${TARGET_NAME} PRIVATE ${Qt5Core_INCLUDE_DIRS}  )
target_include_directories(${TARGET_NAME} PRIVATE ${ZeroMQ_INCLUDE_DIR}    )
target_include_directories(${TARGET_NAME} PRIVATE ${spdlog_INCLUDE_DIRS}   )
target_include_directories(${TARGET_NAME} PRIVATE ${cppzmq_INCLUDE_DIRS}   )
target_include_directories(${TARGET_NAME} PRIVATE ${protobuf_INCLUDE_DIRS} )
target_include_directories(${TARGET_NAME} PRIVATE ${fmt_INCLUDE_DIRS}      )
