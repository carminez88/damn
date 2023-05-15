cmake_minimum_required(VERSION 3.14)

project(DeviceActivityMonitor-VisualizationClient VERSION 0.1 LANGUAGES CXX)

#add_compile_options(/MTd)
if(MSVC)
    add_compile_options (/permissive-)
    add_compile_options (/DNOGDI)
    add_compile_options (/DNOMINMAX)
    add_compile_options (/MP12) # FIXME: hardcoded 12
    add_compile_options ("$<IF:$<CONFIG:Debug>,/MDd,/MD>")
    add_compile_options (/Zc:__cplusplus)
    add_compile_options ("$<IF:$<CONFIG:Debug>,/Od,/O2>")
    add_compile_options (/EHsc)
    add_compile_options (/nologo)
    #add_compile_options (/W4)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-W -Wall -pipe -fPIC -Wextra -msse -msse2 -msse3 -mssse3 -msahf $<IF:$<CONFIG:Debug>,-O0,-O3> $<IF:$<CONFIG:Debug>,-g3,-g0>)
    add_compile_options("$<$<CONFIG:DEBUG>:-Wno-system-headers>")
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)
find_package(ZeroMQ 4.3.4 REQUIRED)
find_package(spdlog REQUIRED)
find_package(Protobuf REQUIRED)
find_package(cppzmq 4.9.0 REQUIRED)
find_package(fmt REQUIRED)

add_subdirectory(proto)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR_DIR}/proto)
include_directories(${CMAKE_CURRENT_SOURCE_DIR_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR_DIR}/src/Dummy)
include_directories(${Protobuf_INCLUDE_DIRS})

set(PROJECT_SOURCES
		src/Dummy/DummyDevicePublisher.h
		src/Dummy/DummyDevicePublisher.cpp
        src/Controller.h
        src/Controller.cpp
        src/DeviceData.h
        src/DeviceData.cpp
        src/DeviceListModel.h
        src/DeviceListModel.cpp
        src/DeviceFilterProxyModel.h
        src/DeviceFilterProxyModel.cpp
        src/EnumConverter.h
        src/Listener.h
        src/Listener.cpp
        src/main.cpp
        src/qml.qrc
        #src/IPacket.h
        src/Packet.h
        #src/Packet.cpp
        #src/PacketType.h
        src/Socket.h
        src/Socket.cpp
        src/ZMQUtils.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(VisualizationClient
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET DeviceMonitor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(VisualizationClient SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(VisualizationClient
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(VisualizationClient PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)
target_link_libraries(VisualizationClient PRIVATE ${ZeroMQ_LIBRARIES}  ${spdlog_LIBRARIES} ${fmt_LIBRARIES} ${cppzmq_LIBRARIES})
target_link_libraries(VisualizationClient PRIVATE ${cppzmq_LIBRARIES} ${ZeroMQ_LIBRARIES} ${protobuf_LIBRARIES} proto)

target_compile_definitions(VisualizationClient PRIVATE ${spdlog_DEFINITIONS} ${fmt_DEFINITIONS})
target_compile_definitions(VisualizationClient PRIVATE ${protobuf_DEFINITIONS})
target_compile_definitions(VisualizationClient PRIVATE ${ZeroMQ_DEFINITIONS})
target_compile_definitions(VisualizationClient PRIVATE ${cppzmq_DEFINITIONS})

target_include_directories(VisualizationClient PRIVATE ${Qt5Core_INCLUDE_DIRS}  )
target_include_directories(VisualizationClient PRIVATE ${ZeroMQ_INCLUDE_DIR}    )
target_include_directories(VisualizationClient PRIVATE ${spdlog_INCLUDE_DIRS}   )
target_include_directories(VisualizationClient PRIVATE ${cppzmq_INCLUDE_DIRS}   )
target_include_directories(VisualizationClient PRIVATE ${protobuf_INCLUDE_DIRS} )
target_include_directories(VisualizationClient PRIVATE ${fmt_INCLUDE_DIRS}      )

set_target_properties(VisualizationClient PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS VisualizationClient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(VisualizationClient)
    qt_finalize_executable(VisualizationClient)
endif()
