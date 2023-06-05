cmake_minimum_required(VERSION 3.14)

project(DamnTest VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include( ../cmake/DAMNCommonOptions.cmake  )
include( ../cmake/DAMNCompileOptions.cmake )
include( ../cmake/DAMNFindQT.cmake         )
include( ../cmake/DAMNFindSpdlog.cmake     )
include( ../cmake/DAMNFindFmt.cmake        )
include( ../cmake/DAMNFindZeroMQ.cmake     )
include( ../cmake/DAMNFindCppZmq.cmake     )
include( ../cmake/DAMNFindCereal.cmake     )
include( ../cmake/DAMNFindGTest.cmake      )

enable_testing()
include(GoogleTest)
include(CTest)

set(tests
    Test_CerealUtils
    Test_EnumConverter
    Test_Looper
    Test_ZMQUtils)

foreach(test ${tests})
    add_executable(${test} src/${test}.cpp)
	
    target_compile_definitions(${test}
      PRIVATE
      ${spdlog_DEFINITIONS}
      ${fmt_DEFINITIONS}
      ${cereal_DEFINITIONS}
      ${GTest_DEFINITIONS})
	  
    target_link_libraries(${test}
      PRIVATE
      ${ZeroMQ_LIBRARIES}
      ${cppzmq_LIBRARIES}
      ${spdlog_LIBRARIES}
      ${fmt_LIBRARIES}
      GTest::gtest_main
      GTest::gmock_main
      DAMN::Common)

    target_include_directories(${test}
      PRIVATE
      ${ZeroMQ_INCLUDE_DIR}
      ${spdlog_INCLUDE_DIRS}
      ${cppzmq_INCLUDE_DIRS}
      ${fmt_INCLUDE_DIRS}
      ${cereal_INCLUDE_DIRS}
      ${GTest_INCLUDE_DIRS}
      ../common/src
      ../common/)

    if(MSVC)
      target_link_options(${test} PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
    endif()

    if(WIN32)
      set_target_properties(${test}
    	PROPERTIES
    	WIN32_EXECUTABLE TRUE)
    endif()
	
    add_test(NAME ${test} COMMAND ${test})
	
    gtest_discover_tests(${test})
	
endforeach()

