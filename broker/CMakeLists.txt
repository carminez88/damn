cmake_minimum_required(VERSION 3.14)

project(Broker VERSION 0.1 LANGUAGES CXX)

include( ../cmake/DAMNFindSpdlog.cmake   )
include( ../cmake/DAMNFindFmt.cmake      )
include( ../cmake/DAMNFindZeroMQ.cmake   )
include( ../cmake/DAMNFindCppZmq.cmake   )
include( ../cmake/DAMNFindProtobuf.cmake )

file(GLOB HDRS "src/*.h")
file(GLOB SRCS "src/*.cpp")

set(PROJECT_SOURCES ${HDRS})
list(APPEND PROJECT_SOURCES ${SRCS})

set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME} ${PROJECT_SOURCES})

target_link_libraries(${TARGET_NAME} PRIVATE ${ZeroMQ_LIBRARIES} ${cppzmq_LIBRARIES} DAMN::Common )

target_include_directories( ${TARGET_NAME} PRIVATE ${ZeroMQ_INCLUDE_DIR}    )
target_include_directories( ${TARGET_NAME} PRIVATE ${spdlog_INCLUDE_DIRS}   )
target_include_directories( ${TARGET_NAME} PRIVATE ${cppzmq_INCLUDE_DIRS}   )
target_include_directories( ${TARGET_NAME} PRIVATE ${protobuf_INCLUDE_DIRS} )
target_include_directories( ${TARGET_NAME} PRIVATE ${fmt_INCLUDE_DIRS}      )
target_include_directories( ${TARGET_NAME} PRIVATE ../common/src            )
target_include_directories( ${TARGET_NAME} PRIVATE ../common/               )

if(MSVC)
    target_link_options(${TARGET_NAME} PRIVATE /ENTRY:mainCRTStartup)
endif()

target_compile_definitions(${TARGET_NAME} PRIVATE ${spdlog_DEFINITIONS} ${fmt_DEFINITIONS})
												  
include_directories( ${CMAKE_CURRENT_BINARY_DIR}     )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}     )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src )

set_target_properties(${TARGET_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

install(TARGETS ${TARGET_NAME} BUNDLE DESTINATION . LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
