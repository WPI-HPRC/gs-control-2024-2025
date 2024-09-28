cmake_minimum_required(VERSION 3.27)
project(gs_backend_2024_2025)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        SerialPort
        WebSockets
        REQUIRED)

add_executable(gs_backend_2024_2025 main.cpp
        Backend/Backend.cpp
        Backend/Backend.h
        Backend/RadioModule.cpp
        Backend/RadioModule.h
        Utility/DataLogger.cpp
        Utility/DataLogger.h
        Constants.h
        Utility/SerialPort.cpp
        Utility/SerialPort.h
        Utility/WebServer.cpp
        Utility/WebServer.h
        Utility/Utility.h
        Utility/json_struct.h
        Utility/json_struct.h
        xbee/XBeeDevice.cpp
        Frontend/Windows/RadioControlsWindow/RadioControlsWindow.cpp
        Frontend/Windows/RadioControlsWindow/RadioControlsWindow.h
        Frontend/Windows/RadioControlsWindow/RadioControlsWindow.ui
        Frontend/Widgets/SerialPortList.cpp
        Frontend/Widgets/SerialPortList.h
        Utility/SerialPortManager.cpp
        Utility/SerialPortManager.h
)
target_link_libraries(gs_backend_2024_2025
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::SerialPort
        Qt6::WebSockets
)

