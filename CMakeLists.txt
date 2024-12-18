cmake_minimum_required(VERSION 3.27)
project(gs_backend_2024_2025)

set(CMAKE_CXX_STANDARD 17)
set(ABSL_PROPAGATE_CXX_STD, ON)

# Add Protobuf as a subdirectory
set(BUILD_SHARED_LIBS OFF)  # Use static libraries for Protobuf
set(protobuf_BUILD_TESTS OFF CACHE BOOL "Disable Protobuf tests")
add_subdirectory(protobuf cmake-protobuf-build)

# Set Protobuf variables
set(Protobuf_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/cmake-protobuf-build/src/protobuf-build/include)
set(Protobuf_LIBRARIES protobuf::libprotobuf)
set(Protobuf_PROTOC_EXECUTABLE $<TARGET_FILE:protoc>)

# Output Protobuf variables for debugging
message(STATUS "Protobuf_INCLUDE_DIRS: ${Protobuf_INCLUDE_DIRS}")
message(STATUS "Protobuf_LIBRARIES: ${Protobuf_LIBRARIES}")
message(STATUS "Protobuf_PROTOC_EXECUTABLE: ${Protobuf_PROTOC_EXECUTABLE}")

# Automatically find all .proto files in a directory
file(GLOB_RECURSE PROTO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/telemetry/*.proto")
list(REMOVE_DUPLICATES PROTO_FILES)  # Ensure no duplicates

# Print all .proto files line-by-line
message(STATUS "Proto Files Found:")
foreach(PROTO_FILE ${PROTO_FILES})
    message(STATUS "  - ${PROTO_FILE}")
endforeach()

# Create lists for the generated sources and headers
set(PROTO_SRCS)
set(PROTO_HDRS)

# Loop over all found .proto files
foreach(PROTO_FILE ${PROTO_FILES})
    # Get the file name without the directory
    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)

    # Get the relative directory of the .proto file from the source folder
    file(RELATIVE_PATH REL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/telemetry ${PROTO_FILE})
    get_filename_component(PROTO_DIR ${REL_PATH} DIRECTORY)

    # Define the output directory for generated files
    # Make sure the output directory structure doesn't get nested unnecessarily
    set(PROTO_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/telemetry)
    file(MAKE_DIRECTORY ${PROTO_OUTPUT_DIR})  # Create the directory if it doesn't exist

    # Log to check if the directories are being resolved correctly
    message(STATUS "Proto File: ${PROTO_FILE}")
    message(STATUS "Output Directory: ${PROTO_OUTPUT_DIR}")

    # Generate Protobuf files
    add_custom_command(
            OUTPUT ${PROTO_OUTPUT_DIR}/${PROTO_NAME}.pb.cc
            ${PROTO_OUTPUT_DIR}/${PROTO_NAME}.pb.h
            COMMAND ${Protobuf_PROTOC_EXECUTABLE}
            --cpp_out=${PROTO_OUTPUT_DIR}
            --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/telemetry
            ${PROTO_FILE}
            DEPENDS ${PROTO_FILE}
            COMMENT "Generating Protobuf files for ${PROTO_FILE}"
    )

    # Append the generated source file to the list
    list(APPEND PROTO_SRCS ${PROTO_OUTPUT_DIR}/${PROTO_NAME}.pb.cc)
endforeach()

# Include directories for generated headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${PROTO_HDRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk)

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

# Add the executable
add_executable(gs_backend_2024_2025
        ${PROTO_SRCS}
        main.cpp
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
        Utility/DataSimulator/DataSimulator.cpp
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
        Frontend/Windows/LogWindow/logwindow.cpp
        Frontend/Windows/LogWindow/logwindow.h
        Frontend/Windows/LogWindow/logwindow.ui
        Utility/TelemetryHandler.cpp
        Utility/TelemetryHandler.h
        Frontend/Widgets/DataSimulationButton/DataSimulationButton.cpp
        Frontend/Widgets/DataSimulationButton/DataSimulationButton.h
        Utility/UnitConversions.h
)

# Link Protobuf and Qt6 libraries
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
        ${Protobuf_LIBRARIES}
)

# Include all generated files in the target
target_sources(gs_backend_2024_2025 PRIVATE ${PROTO_SRCS})