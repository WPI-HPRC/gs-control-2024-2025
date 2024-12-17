cmake_minimum_required(VERSION 3.27)
project(gs_backend_2024_2025)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        SerialPort
        WebSockets
        3DCore
        3DExtras
        3DInput
        3DRender
        REQUIRED)

add_executable(gs_backend_2024_2025 main.cpp
        Resources/Resources.qrc
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
        Frontend/Windows/TelemetryWindow/raw_telemetry.cpp
        Frontend/Windows/TelemetryWindow/raw_telemetry.h
        Frontend/Windows/TelemetryWindow/raw_telemetry.ui
        Frontend/Widgets/TimeDateWidgets/datewidget.cpp
        Frontend/Widgets/TimeDateWidgets/datewidget.h
        Frontend/Widgets/TimeDateWidgets/datewidget.ui
        Frontend/Widgets/TimeDateWidgets/groundtimewidget.cpp
        Frontend/Widgets/TimeDateWidgets/groundtimewidget.h
        Frontend/Widgets/TimeDateWidgets/groundtimewidget.h
        Frontend/Widgets/TimeDateWidgets/datewidget.h
        Frontend/Widgets/TimeDateWidgets/datewidget.h
        Frontend/Widgets/TimeDateWidgets/groundtimewidget.h
        Frontend/Widgets/TimeDateWidgets/rocketuptimewidget.cpp
        Frontend/Widgets/TimeDateWidgets/rocketuptimewidget.h
        Frontend/Widgets/TimeDateWidgets/rocketuptimewidget.h
        Frontend/Widgets/TimeDateWidgets/payloaduptimewidget.h
        Frontend/Widgets/TimeDateWidgets/payloaduptimewidget.cpp
        Frontend/Widgets/TimeDateWidgets/groundflighttimewidget.h
        Frontend/Widgets/TimeDateWidgets/groundflighttimewidget.cpp
        Frontend/Widgets/TimeDateWidgets/datewidget.cpp
        Frontend/Widgets/TimeDateWidgets/datewidget.h
        Frontend/Widgets/TimeDateWidgets/datewidget.ui
        Utility/DataSimulator.cpp
        Frontend/Widgets/TimeDateWidgets/datewidget.cpp
        Frontend/Widgets/TimeDateWidgets/datewidget.h
        Frontend/Widgets/TimeDateWidgets/datewidget.ui
        Frontend/Widgets/TimeDateWidgets/groundtimewidget.cpp
        Frontend/Widgets/TimeDateWidgets/groundtimewidget.h
        Frontend/Widgets/TimeDateWidgets/groundtimewidget.h
        Frontend/Widgets/TimeDateWidgets/datewidget.h
        Frontend/Widgets/TimeDateWidgets/datewidget.h
        Frontend/Widgets/TimeDateWidgets/groundtimewidget.h
        Frontend/Widgets/TimeDateWidgets/rocketuptimewidget.cpp
        Frontend/Widgets/TimeDateWidgets/rocketuptimewidget.h
        Frontend/Widgets/TimeDateWidgets/rocketuptimewidget.h
        Frontend/Widgets/TimeDateWidgets/payloaduptimewidget.h
        Frontend/Widgets/TimeDateWidgets/payloaduptimewidget.cpp
        Frontend/Widgets/TimeDateWidgets/groundflighttimewidget.h
        Frontend/Widgets/TimeDateWidgets/groundflighttimewidget.cpp
        Frontend/Widgets/TimeDateWidgets/rocketflighttimewidget.h
        Frontend/Widgets/TimeDateWidgets/rocketflighttimewidget.cpp
        Frontend/Widgets/RadioPerformanceWidgets/bytespersecwidget.cpp
        Frontend/Widgets/RadioPerformanceWidgets/bytespersecwidget.h
        Frontend/Widgets/RadioPerformanceWidgets/packetspersecwidget.cpp
        Frontend/Widgets/RadioPerformanceWidgets/packetspersecwidget.h
        Frontend/Widgets/RadioPerformanceWidgets/droppedpacketswidget.cpp
        Frontend/Widgets/RadioPerformanceWidgets/droppedpacketswidget.h
        Frontend/Widgets/RadioPerformanceWidgets/rssiwidget.cpp
        Frontend/Widgets/RadioPerformanceWidgets/rssiwidget.h
        Frontend/Windows/RadioPerformanceWindow/radioperformancewindow.cpp
        Frontend/Windows/RadioPerformanceWindow/radioperformancewindow.h
        Frontend/Windows/RadioPerformanceWindow/radioperformancewindow.ui
        Frontend/Windows/TelemetryWindow/raw_telemetry.cpp
        Frontend/Windows/TelemetryWindow/raw_telemetry.h
        Frontend/Windows/TelemetryWindow/raw_telemetry.ui
        Frontend/Widgets/Viewer3D/Viewer3D.cpp
        Frontend/Widgets/Viewer3D/Viewer3D.h
        Frontend/Windows/MainWindow/mainwindow.cpp
        Frontend/Windows/MainWindow/mainwindow.h
        Frontend/Windows/MainWindow/mainwindow.ui
        Frontend/Widgets/IndicatorWidget/IndicatorWidget/QLedLabel.cpp
        Frontend/Widgets/IndicatorWidget/IndicatorWidget/QLedLabel.h
        Frontend/Widgets/IndicatorWidget/IndicatorWidget/ledwidget.cpp
        Frontend/Widgets/IndicatorWidget/IndicatorWidget/ledwidget.h
        Frontend/Widgets/IndicatorWidget/IndicatorWidget/ledwidget.ui
        Frontend/Widgets/IndicatorWidget/RocketIndicator/RocketConnectionIndicator.cpp
        Frontend/Widgets/IndicatorWidget/RocketIndicator/RocketConnectionIndicator.h
        Frontend/Widgets/IndicatorWidget/PayloadIndicator/PayloadConnectionIndicator.cpp
        Frontend/Widgets/IndicatorWidget/PayloadIndicator/PayloadConnectionIndicator.h
)
target_link_libraries(gs_backend_2024_2025
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::SerialPort
        Qt6::WebSockets
        Qt6::3DCore
        Qt6::3DExtras
        Qt6::3DInput
        Qt6::3DRender
)

