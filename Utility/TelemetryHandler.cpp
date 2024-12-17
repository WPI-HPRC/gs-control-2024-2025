//
// Created by William Scheirey on 12/1/24.
//

#include "TelemetryHandler.h"
#include "google/protobuf/dynamic_message.h"
#include <iostream>
#include <google/protobuf/descriptor.h>

bool TelemetryHandler::setField(google::protobuf::Message *message, const google::protobuf::Reflection* reflection, const google::protobuf::Descriptor* descriptor, const std::string& fieldName, const std::string& value) {
    const google::protobuf::FieldDescriptor* fieldDescriptor = descriptor->FindFieldByName(fieldName);

    if (!fieldDescriptor) {
        return false;
    }

    switch (fieldDescriptor->type()) {
        case google::protobuf::FieldDescriptor::TYPE_INT32: {
            try {
                int32_t intValue = std::stoi(value);
                reflection->SetInt32(message, fieldDescriptor, intValue);
            } catch (...) {
                return false;
            }
            break;
        }
        case google::protobuf::FieldDescriptor::TYPE_INT64: {
            try {
                int64_t int64Value = std::stoll(value);
                reflection->SetInt64(message, fieldDescriptor, int64Value);
            } catch (...) {
                return false;
            }
            break;
        }
        case google::protobuf::FieldDescriptor::TYPE_UINT32: {
            try {
                uint32_t uint32Value = std::stoul(value);
                reflection->SetUInt32(message, fieldDescriptor, uint32Value);
            } catch (...) {
                return false;
            }
            break;
        }
        case google::protobuf::FieldDescriptor::TYPE_UINT64: {
            try {
                uint64_t uint64Value = std::stoull(value);
                reflection->SetUInt64(message, fieldDescriptor, uint64Value);
            } catch (...) {
                return false;
            }
            break;
        }
        case google::protobuf::FieldDescriptor::TYPE_FLOAT: {
            try {
                float floatValue = std::stof(value);
                reflection->SetFloat(message, fieldDescriptor, floatValue);
            } catch (...) {
                return false;
            }
            break;
        }
        case google::protobuf::FieldDescriptor::TYPE_DOUBLE: {
            try {
                double doubleValue = std::stod(value);
                reflection->SetDouble(message, fieldDescriptor, doubleValue);
            } catch (...) {
                return false;
            }
            break;
        }
        case google::protobuf::FieldDescriptor::TYPE_STRING: {
            reflection->SetString(message, fieldDescriptor, value);
            break;
        }
        case google::protobuf::FieldDescriptor::TYPE_BOOL: {
            if (value == "true" || value == "1") {
                reflection->SetBool(message, fieldDescriptor, true);
            } else if (value == "false" || value == "0") {
                reflection->SetBool(message, fieldDescriptor, false);
            } else {
                return false;
            }
            break;
        }
        default:
            return false;
    }

    return true;
}



TelemetryHandler::TelemetryHandler()
{

}