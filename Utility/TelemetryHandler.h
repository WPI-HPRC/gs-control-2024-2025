//
// Created by William Scheirey on 12/1/24.
//

#ifndef GS_BACKEND_2024_2025_TELEMETRYHANDLER_H
#define GS_BACKEND_2024_2025_TELEMETRYHANDLER_H

#include "generated/telemetry/RocketTelemetryPacket.pb.h"
#include <google/protobuf/reflection.h>

class TelemetryHandler
{
public:
    static bool setField(google::protobuf::Message *message, const google::protobuf::Reflection *reflection, const google::protobuf::Descriptor *descriptor, const std::string &fieldName, const std::string &value);

    static TelemetryHandler &getInstance()
    {
        static TelemetryHandler instance;
        return instance;
    }
    TelemetryHandler(const TelemetryHandler&) = delete;
    TelemetryHandler &operator=(const TelemetryHandler&) = delete;

private:
    TelemetryHandler();
};


#endif //GS_BACKEND_2024_2025_TELEMETRYHANDLER_H
