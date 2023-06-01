cmake_minimum_required( VERSION 3.20 )

project( Common CXX )

include( ../cmake/DAMNCompileOptions.cmake )
include( ../cmake/DAMNCommonOptions.cmake  )
include( ../cmake/DAMNFindZeroMQ.cmake     )
include( ../cmake/DAMNFindCppZmq.cmake     )
include( ../cmake/DAMNFindSpdlog.cmake     )
include( ../cmake/DAMNFindFmt.cmake        )
include( ../cmake/DAMNFindCereal.cmake     )

set(TARGET_NAME ${PROJECT_NAME})

file(GLOB HDRS "src/*.h")
file(GLOB SRCS "src/*.cpp")

add_library ( ${TARGET_NAME} STATIC ${HDRS} ${SRCS} )
add_library ( DAMN::Common ALIAS ${TARGET_NAME}     )

target_link_libraries(${TARGET_NAME}
  PRIVATE
  ${ZeroMQ_LIBRARIES} 
  ${spdlog_LIBRARIES}
  ${fmt_LIBRARIES}
  ${cppzmq_LIBRARIES}
  ${cereal_LIBRARIES})

target_compile_definitions(${TARGET_NAME}
  PRIVATE
  ${spdlog_DEFINITIONS} 
  ${fmt_DEFINITIONS}
  ${ZeroMQ_DEFINITIONS}
  ${cppzmq_DEFINITIONS}
  ${cereal_DEFINITIONS})

include_directories(${CMAKE_CURRENT_BINARY_DIR}    )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}    )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

target_include_directories(${TARGET_NAME}
  PRIVATE 
  ${Qt5Core_INCLUDE_DIRS}
  ${ZeroMQ_INCLUDE_DIR}
  ${spdlog_INCLUDE_DIRS} 
  ${cppzmq_INCLUDE_DIRS} 
  ${fmt_INCLUDE_DIRS}    
  ${cereal_INCLUDE_DIRS})