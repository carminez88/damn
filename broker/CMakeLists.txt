cmake_minimum_required(VERSION 3.14)

project(Broker VERSION 0.1 LANGUAGES CXX)

include( ../cmake/DAMNCommonOptions.cmake  )
include( ../cmake/DAMNCompileOptions.cmake )
include( ../cmake/DAMNFindSpdlog.cmake     )
include( ../cmake/DAMNFindFmt.cmake        )
include( ../cmake/DAMNFindZeroMQ.cmake     )
include( ../cmake/DAMNFindCppZmq.cmake     )
include( ../cmake/DAMNFindCLI11.cmake      )

file(GLOB HDRS "src/*.h")
file(GLOB SRCS "src/*.cpp")

set(PROJECT_SOURCES ${HDRS})
list(APPEND PROJECT_SOURCES ${SRCS})

set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME} ${PROJECT_SOURCES})

target_link_libraries(${TARGET_NAME}
  PRIVATE
  ${ZeroMQ_LIBRARIES}
  ${cppzmq_LIBRARIES}
  ${CLI11_LIBRARIES}
  ${spdlog_LIBRARIES}
  ${fmt_LIBRARIES}
  DAMN::Common)

target_include_directories(${TARGET_NAME}
  PRIVATE
  ${ZeroMQ_INCLUDE_DIR}
  ${spdlog_INCLUDE_DIRS}
  ${cppzmq_INCLUDE_DIRS}
  ${fmt_INCLUDE_DIRS}
  ${CLI11_INCLUDE_DIRS}
  ../common/src
  ../common/)

if(MSVC)
    target_link_options(${TARGET_NAME} PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
endif()

target_compile_definitions(${TARGET_NAME}
  PRIVATE
  ${spdlog_DEFINITIONS}
  ${fmt_DEFINITIONS}
  ${CLI11_DEFINITIONS})
												  
include_directories( ${CMAKE_CURRENT_BINARY_DIR}     )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}     )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src )

if(WIN32)
  set_target_properties(${TARGET_NAME}
    PROPERTIES
    WIN32_EXECUTABLE TRUE)
endif()

install(TARGETS ${TARGET_NAME} BUNDLE DESTINATION . LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
