cmake_minimum_required(VERSION 3.27)
project(gs_backend_2024_2025)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        SerialPort
        WebSockets
        REQUIRED)

add_executable(gs_backend_2024_2025 main.cpp
        Backend.cpp
        Backend.h
        RadioModule.cpp
        RadioModule.h
        DataLogger.cpp
        DataLogger.h
        Constants.h
        SerialPort.cpp
        SerialPort.h
        WebServer.cpp
        WebServer.h
        Utility.h
        json_struct.h
        json_struct.h
        xbee/XBeeDevice.cpp
)
target_link_libraries(gs_backend_2024_2025
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::SerialPort
        Qt6::WebSockets
)

