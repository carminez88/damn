cmake_minimum_required(VERSION 3.14)

project(VisualizationClient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include( ../../cmake/DAMNFindQT.cmake       )
include( ../../cmake/DAMNFindSpdlog.cmake   )
include( ../../cmake/DAMNFindFmt.cmake      )
include( ../../cmake/DAMNFindZeroMQ.cmake   )
include( ../../cmake/DAMNFindCppZmq.cmake   )
include( ../../cmake/DAMNFindProtobuf.cmake )

file(GLOB HDRS "src/*.h" "src/dummy/*.h")
file(GLOB SRCS "src/*.cpp" "src/dummy/*.cpp")

set(PROJECT_SOURCES ${HDRS})
list(APPEND PROJECT_SOURCES ${SRCS})

set(TARGET_NAME ${PROJECT_NAME})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${TARGET_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES})
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET DeviceMonitor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${TARGET_NAME} SHARED ${PROJECT_SOURCES})
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${TARGET_NAME} ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(${TARGET_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)
target_link_libraries(${TARGET_NAME} PRIVATE DAMN::Common                                            )

target_include_directories( ${TARGET_NAME} PRIVATE ${Qt5Core_INCLUDE_DIRS}  )
target_include_directories( ${TARGET_NAME} PRIVATE ${ZeroMQ_INCLUDE_DIR}    )
target_include_directories( ${TARGET_NAME} PRIVATE ${spdlog_INCLUDE_DIRS}   )
target_include_directories( ${TARGET_NAME} PRIVATE ${cppzmq_INCLUDE_DIRS}   )
target_include_directories( ${TARGET_NAME} PRIVATE ${protobuf_INCLUDE_DIRS} )
target_include_directories( ${TARGET_NAME} PRIVATE ${fmt_INCLUDE_DIRS}      )
target_include_directories( ${TARGET_NAME} PRIVATE ../../common/src         )
target_include_directories( ${TARGET_NAME} PRIVATE ../../common/            )

target_compile_definitions(${TARGET_NAME} PRIVATE ${spdlog_DEFINITIONS} ${fmt_DEFINITIONS})
												  
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/dummy)

set_target_properties(${TARGET_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${TARGET_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(${TARGET_NAME})
    qt_finalize_executable(${TARGET_NAME})
endif()
