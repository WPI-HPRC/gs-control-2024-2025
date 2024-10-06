//
// Created by William Scheirey on 8/20/24.
//

#ifndef GS_BACKEND_2024_2025_UTILITY_H
#define GS_BACKEND_2024_2025_UTILITY_H

#include "json_struct.h"

#define GROUND_STATION_ADDR 0x0013A200423F474C

namespace GroundStation
{
    enum PacketType
    {
        Unknown = -1,
        Rocket = 0x01,
        Payload = 0x02
    };

#pragma pack(push, 1)

    struct RocketTelemPacket
    {
        // State Integer
        // 0 - PreLaunch
        // 1 - Launch
        // 2 - Coast
        // 3 - DrogueDescent
        // 4 - MainDescent
        // 5 - Recovery
        // 6 - Abort
        uint8_t state = 0;
        // Raw Sensor Readings
        float accelX = 0.0f;
        float accelY = 0.0f;
        float accelZ = 0.0f;
        float gyroX = 0.0f;
        float gyroY = 0.0f;
        float gyroZ = 0.0f;
        float rawMagX = 0.0f;
        float rawMagY = 0.0f;
        float rawMagZ = 0.0f;
        float pressure = 0.0f;
        float temperature = 0.0f;

        uint32_t servoPosition = 0;

        // Calculated Values
        float altitude = 0.0f;
        float magX = 0.0f;
        float magY = 0.0f;
        float magZ = 0.0f;

        // EKF Results
        float w = 0.0f; // Quaternion State
        float i = 0.0f;
        float j = 0.0f;
        float k = 0.0f;
        float posX = 0.0f; // Position State ECEF
        float posY = 0.0f;
        float posZ = 0.0f;
        float velX = 0.0f; // Velocity State ECEF
        float velY = 0.0f;
        float velZ = 0.0f;

        // GPS Inputs
        float gpsLat = 0.0f;
        float gpsLong = 0.0f;
        float gpsAltMSL = 0.0f;
        float gpsAltAGL = 0.0f;
        int32_t gpsVelocityN = 0;
        int32_t gpsVelocityE = 0;
        int32_t gpsVelocityD = 0;
        uint32_t epochTime = 0;
        uint8_t satellites = 0;
        bool gpsLock = false;

        uint32_t loopCount = 0;
        uint32_t timestamp = 0;

        JS_OBJ(
                state,

                accelX,
                accelY,
                accelZ,

                gyroX,
                gyroY,
                gyroZ,

                rawMagX,
                rawMagY,
                rawMagZ,

                pressure,
                temperature,
                servoPosition,
                altitude,

                magX,
                magY,
                magZ,

                w,
                i,
                j,
                k,

                posX,
                posY,
                posZ,

                velX,
                velY,
                velZ,

                gpsLat,
                gpsLong,
                gpsAltMSL,
                gpsAltAGL,
                gpsVelocityN,
                gpsVelocityE,
                gpsVelocityD,

                epochTime,
                satellites,
                gpsLock,

                loopCount,
                timestamp
        );
    };

    struct PayloadTelemPacket
    {
        // State Integer
        // 0 - PreLaunch
        // 1 - Test
        uint8_t p_state = 0;

        // Raw Sensor Readings
        float p_accelX = 0.0f;
        float p_accelY = 0.0f;
        float p_accelZ = 0.0f;
        float p_gyroX = 0.0f;
        float p_gyroY = 0.0f;
        float p_gyroZ = 0.0f;
        float p_rawMagX = 0.0f;
        float p_rawMagY = 0.0f;
        float p_rawMagZ = 0.0f;
        float p_pressure = 0.0f;
        float p_temperature = 0.0f;

        // Calculated Values
        float p_altitude = 0.0f;
        float p_magX = 0.0f;
        float p_magY = 0.0f;
        float p_magZ = 0.0f;

        // EKF Results
        float p_w = 0.0f; // Quaternion State
        float p_i = 0.0f;
        float p_j = 0.0f;
        float p_k = 0.0f;
        float p_posX = 0.0f; // Position State ECEF
        float p_posY = 0.0f;
        float p_posZ = 0.0f;
        float p_velX = 0.0f; // Velocity State ECEF
        float p_velY = 0.0f;
        float p_velZ = 0.0f;

        // GPS Inputs
        float p_gpsLat = 0.0f;
        float p_gpsLong = 0.0f;
        float p_gpsAltMSL = 0.0f;
        float p_gpsAltAGL = 0.0f;
        int32_t p_gpsVelocityN = 0;
        int32_t p_gpsVelocityE = 0;
        int32_t p_gpsVelocityD = 0;
        uint32_t p_epochTime = 0;
        uint8_t p_satellites = 0;
        bool p_gpsLock = false;

        uint32_t p_loopCount = 0;
        uint32_t p_timestamp = 0;

        //Payload Specific

        //CV
        uint32_t p_cx = 0; //Camera Centroids
        uint32_t p_cy = 0;

        float p_targetGpsLat = 0.0f; //Target Point GPS Estimations
        float p_targetGpsLong = 0.0f;

        //Controls
        uint32_t p_desiredServoPos1 = 0; //Servo Controls Values
        uint32_t p_actualServoPos1 = 0;
        uint32_t p_desiredServoPos2 = 0;
        uint32_t p_actualServoPos2 = 0;
        uint32_t p_desiredServoPos3 = 0;
        uint32_t p_actualServoPos3 = 0;
        uint32_t p_desiredServoPos4 = 0;
        uint32_t p_actualServoPos4 = 0;

        float p_trajA = 0.0f; //Calculated Trajectory Constants
        float p_trajB = 0.0f;
        float p_trajC = 0.0f;
        float p_trajD = 0.0f;

        JS_OBJ(
                p_state,

                p_accelX,
                p_accelY,
                p_accelZ,

                p_gyroX,
                p_gyroY,
                p_gyroZ,

                p_rawMagX,
                p_rawMagY,
                p_rawMagZ,

                p_pressure,
                p_temperature,
                p_altitude,

                p_magX,
                p_magY,
                p_magZ,

                p_w,
                p_i,
                p_j,
                p_k,

                p_posX,
                p_posY,
                p_posZ,

                p_velX,
                p_velY,
                p_velZ,

                p_gpsLat,
                p_gpsLong,
                p_gpsAltMSL,
                p_gpsAltAGL,
                p_gpsVelocityN,
                p_gpsVelocityE,
                p_gpsVelocityD,
                p_epochTime,
                p_satellites,
                p_gpsLock,

                p_loopCount,
                p_timestamp,

                p_cx,
                p_cy,

                p_targetGpsLat,
                p_targetGpsLong,

                p_desiredServoPos1,
                p_actualServoPos1,
                p_desiredServoPos2,
                p_actualServoPos2,
                p_desiredServoPos3,
                p_actualServoPos3,
                p_desiredServoPos4,
                p_actualServoPos4,

                p_trajA,
                p_trajB,
                p_trajC,
                p_trajD,

                p_loopCount
        );
    };

// For testing purposes
    struct RocketTxPacket
    {
        uint8_t packetType = 0x01;
        // State Integer
        // 0 - PreLaunch
        // 1 - Launch
        // 2 - Coast
        // 3 - DrogueDescent
        // 4 - MainDescent
        // 5 - Recovery
        // 6 - Abort
        uint8_t state = 0;
        // Raw Sensor Readings
        float accelX = 0.0f;
        float accelY = 0.0f;
        float accelZ = 0.0f;
        float gyroX = 0.0f;
        float gyroY = 0.0f;
        float gyroZ = 0.0f;
        float rawMagX = 0.0f;
        float rawMagY = 0.0f;
        float rawMagZ = 0.0f;
        float pressure = 0.0f;

        uint32_t servoPosition = 0;

        // Calculated Values
        float altitude = 0.0f;
        float magX = 0.0f;
        float magY = 0.0f;
        float magZ = 0.0f;

        // EKF Results
        float w = 0.0f; // Quaternion State
        float i = 0.0f;
        float j = 0.0f;
        float k = 0.0f;
        float posX = 0.0f; // Position State ECEF
        float posY = 0.0f;
        float posZ = 0.0f;
        float velX = 0.0f; // Velocity State ECEF
        float velY = 0.0f;
        float velZ = 0.0f;

        // GPS Inputs
        float gpsLat = 0.0f;
        float gpsLong = 0.0f;
        float gpsAltMSL = 0.0f;
        float gpsAltAGL = 0.0f;
        uint32_t epochTime = 0;
        uint8_t satellites = 0;
        bool gpsLock = false;

        uint32_t loopCount = 0;
        uint32_t timestamp = 0;

        JS_OBJ(
                state,
                accelX,
                accelY,
                accelZ,
                gyroX,
                gyroY,
                gyroZ,
                rawMagX,
                rawMagY,
                rawMagZ,
                pressure,
                servoPosition,
                altitude,
                magX,
                magY,
                magZ,
                w,
                i,
                j,
                k,
                posX,
                posY,
                posZ,
                velX,
                velY,
                velZ,
                gpsLat,
                gpsLong,
                gpsAltMSL,
                gpsAltAGL,
                epochTime,
                satellites,
                gpsLock,
                loopCount,
                timestamp
        );
    };
    struct PayloadTxPacket
    {
        uint8_t packetType = 0x02;
        // State Integer
        // 0 - PreLaunch
        // 1 - Launch
        // 2 - Coast
        // 3 - DrogueDescent
        // 4 - MainDescent
        // 5 - Recovery
        // 6 - Abort
        uint8_t state = 0;
        // Raw Sensor Readings
        float accelX = 0.0f;
        float accelY = 0.0f;
        float accelZ = 0.0f;
        float gyroX = 0.0f;
        float gyroY = 0.0f;
        float gyroZ = 0.0f;
        float rawMagX = 0.0f;
        float rawMagY = 0.0f;
        float rawMagZ = 0.0f;
        float pressure = 0.0f;

        uint32_t servoPosition = 0;

        // Calculated Values
        float altitude = 0.0f;
        float magX = 0.0f;
        float magY = 0.0f;
        float magZ = 0.0f;

        // EKF Results
        float w = 0.0f; // Quaternion State
        float i = 0.0f;
        float j = 0.0f;
        float k = 0.0f;
        float posX = 0.0f; // Position State ECEF
        float posY = 0.0f;
        float posZ = 0.0f;
        float velX = 0.0f; // Velocity State ECEF
        float velY = 0.0f;
        float velZ = 0.0f;

        // GPS Inputs
        float gpsLat = 0.0f;
        float gpsLong = 0.0f;
        float gpsAltMSL = 0.0f;
        float gpsAltAGL = 0.0f;
        uint32_t epochTime = 0;
        uint8_t satellites = 0;
        bool gpsLock = false;

        uint32_t loopCount = 0;
        uint32_t timestamp = 0;

        JS_OBJ(
                state,
                accelX,
                accelY,
                accelZ,
                gyroX,
                gyroY,
                gyroZ,
                rawMagX,
                rawMagY,
                rawMagZ,
                pressure,
                servoPosition,
                altitude,
                magX,
                magY,
                magZ,
                w,
                i,
                j,
                k,
                posX,
                posY,
                posZ,
                velX,
                velY,
                velZ,
                gpsLat,
                gpsLong,
                gpsAltMSL,
                gpsAltAGL,
                epochTime,
                satellites,
                gpsLock,
                loopCount,
                timestamp
        );
    };
//struct PayloadTelemPacket
//{
//    // State Integer
//    // 0 - PreLaunch
//    // 1 - Launch
//    // 2 - Coast
//    // 3 - DrogueDescent
//    // 4 - MainDescent
//    // 5 - Recovery
//    // 6 - Abort
//    uint8_t p_state = 0;
//    // Raw Sensor Readings
//    float p_accelX = 0.0f;
//    float p_accelY = 0.0f;
//    float p_accelZ = 0.0f;
//    float p_gyroX = 0.0f;
//    float p_gyroY = 0.0f;
//    float p_gyroZ = 0.0f;
//    float p_rawMagX = 0.0f;
//    float p_rawMagY = 0.0f;
//    float p_rawMagZ = 0.0f;
//    float p_pressure = 0.0f;
//
//    uint32_t p_servoPosition = 0;
//
//    // Calculated Values
//    float p_altitude = 0.0f;
//    float p_magX = 0.0f;
//    float p_magY = 0.0f;
//    float p_magZ = 0.0f;
//
//    // EKF Results
//    float p_w = 0.0f; // Quaternion State
//    float p_i = 0.0f;
//    float p_j = 0.0f;
//    float p_k = 0.0f;
//    float p_posX = 0.0f; // Position State ECEF
//    float p_posY = 0.0f;
//    float p_posZ = 0.0f;
//    float p_velX = 0.0f; // Velocity State ECEF
//    float p_velY = 0.0f;
//    float p_velZ = 0.0f;
//
//    // GPS Inputs
//    float p_gpsLat = 0.0f;
//    float p_gpsLong = 0.0f;
//    float p_gpsAltMSL = 0.0f;
//    float p_gpsAltAGL = 0.0f;
//    uint32_t p_epochTime = 0;
//    uint8_t p_satellites = 0;
//    bool p_gpsLock = false;
//
//    uint32_t p_loopCount = 0;
//    uint32_t p_timestamp = 0;
//
//    JS_OBJ(
//            p_state,
//            p_accelX,
//            p_accelY,
//            p_accelZ,
//            p_gyroX,
//            p_gyroY,
//            p_gyroZ,
//            p_rawMagX,
//            p_rawMagY,
//            p_rawMagZ,
//            p_pressure,
//            p_servoPosition,
//            p_altitude,
//            p_magX,
//            p_magY,
//            p_magZ,
//            p_w,
//            p_i,
//            p_j,
//            p_k,
//            p_posX,
//            p_posY,
//            p_posZ,
//            p_velX,
//            p_velY,
//            p_velZ,
//            p_gpsLat,
//            p_gpsLong,
//            p_gpsAltMSL,
//            p_gpsAltAGL,
//            p_epochTime,
//            p_satellites,
//            p_gpsLock,
//            p_loopCount,
//            p_timestamp
//    );
//};


#pragma pack(pop)
}

#endif //GS_BACKEND_2024_2025_UTILITY_H
