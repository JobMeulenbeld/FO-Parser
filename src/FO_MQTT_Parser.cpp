//
// Created by Arief on 10/03/2023.
//

#include "FO_MQTT_Parser.h"

FO_MQTT_Parser::FO_MQTT_Parser(std::string host, std::string client_id) :
m_buffer{},
m_size{},
m_packetheader()
{
    initialize_event_data();

}

int FO_MQTT_Parser::parse(char *buffer, unsigned long size) {
    m_buffer = buffer;
    m_size = size;
    if(parse_packet_header() < 0){return -1;}

    switch (m_packetheader.m_packetId) {
        case 0: {
            parse_motion_data();
            break;
        }
        case 1: {
            parse_session_data();
            break;
        }
        case 2: {
            parse_lap_data();
            break;
        }
        case 3: {
            //TODO: Check Event
            parse_event_data();

            break;
        }
        case 4: {
            parse_participant_data();
            break;
        }
        case 5: {
            // TODO: Check Car setup
            parse_car_setups();
            break;
        }
        case 6: {
            parse_car_telemetry();
            break;
        }
        case 7: {
            //TODO: Check car status
            parse_car_status();
            break;
        }
        case 8: {
            // TODO: Check Final Classification
            parse_final_classification_data();
            break;
        }
        case 9: {
            //TODO: Check Lobby info
            parse_lobby_info();
            break;
        }
        case 10: {
            // TODO: Check Car damage
            parse_car_damage();
            break;
        }
        case 11: {
            //TODO: Check Session history
            parse_session_history();
            break;
        }
    }
    return 0;
}

int FO_MQTT_Parser::parse_packet_header() {

    if(m_size < 24){return -1;}

    memcpy(&m_packetheader.m_packetFormat, &m_buffer[0], 2);
    memcpy(&m_packetheader.m_gameMajorVersion, &m_buffer[2], 1);
    memcpy(&m_packetheader.m_gameMinorVersion, &m_buffer[3], 1);
    memcpy(&m_packetheader.m_packetVersion, &m_buffer[4], 1);
    memcpy(&m_packetheader.m_packetId, &m_buffer[5], 1);
    memcpy(&m_packetheader.m_sessionUID, &m_buffer[6], 8);
    memcpy(&m_packetheader.m_sessionTime, &m_buffer[14], 4);
    memcpy(&m_packetheader.m_frameIdentifier, &m_buffer[18], 4);
    memcpy(&m_packetheader.m_playerCarIndex, &m_buffer[22], 1);
    memcpy(&m_packetheader.m_secondaryPlayerCarIndex, &m_buffer[23], 1);

    if(debug_packet)
    {
        std::cout << "Packet format:" << m_packetheader.m_packetFormat << std::endl;
        std::cout << "Packet major:" << m_packetheader.m_gameMajorVersion << std::endl;
        std::cout << "Packet minor:" << m_packetheader.m_gameMinorVersion << std::endl;
        std::cout << "Packet packet version:" << m_packetheader.m_packetVersion << std::endl;
        std::cout << "Packet packet id:" << m_packetheader.m_packetId << std::endl;
        std::cout << "Packet uid:" << m_packetheader.m_sessionUID << std::endl;
        std::cout << "Packet time:" << m_packetheader.m_sessionTime << std::endl;
        std::cout << "Packet Iframe:" << m_packetheader.m_frameIdentifier << std::endl;
        std::cout << "Packet player:" << m_packetheader.m_playerCarIndex << std::endl;
        std::cout << "Packet secondary:" << m_packetheader.m_secondaryPlayerCarIndex << std::endl;
    }

    return 0;
}

int FO_MQTT_Parser::parse_motion_data() {

    // Check size
    if (m_size < 1464){return -1;}

    // Calculate index
    int index = 24;

    // Parse byte array

    // Parse CarMotionData
    for(int i = 0; i < 22; i++)
    {
        memcpy(&m_packet_motion.m_carMotionData[i].m_worldPositionX, &m_buffer[index + 0 + (i * 60)], 4);
        memcpy(&m_packet_motion.m_carMotionData[i].m_worldPositionY, &m_buffer[index + 4 + (i * 60)], 4);
        memcpy(&m_packet_motion.m_carMotionData[i].m_worldPositionZ, &m_buffer[index + 8 + (i * 60)], 4);
        memcpy(&m_packet_motion.m_carMotionData[i].m_worldVelocityX, &m_buffer[index + 12 + (i * 60)], 4);
        memcpy(&m_packet_motion.m_carMotionData[i].m_worldVelocityY, &m_buffer[index + 16 + (i * 60)], 4);
        memcpy(&m_packet_motion.m_carMotionData[i].m_worldVelocityZ, &m_buffer[index + 20 + (i * 60)], 4);
        memcpy(&m_packet_motion.m_carMotionData[i].m_worldForwardDirX, &m_buffer[index + 24 + (i * 60)], 2);
        memcpy(&m_packet_motion.m_carMotionData[i].m_worldForwardDirY, &m_buffer[index + 26 + (i * 60)], 2);
        memcpy(&m_packet_motion.m_carMotionData[i].m_worldForwardDirZ, &m_buffer[index + 28 + (i * 60)], 2);
        memcpy(&m_packet_motion.m_carMotionData[i].m_worldRightDirX, &m_buffer[index + 30 + (i * 60)], 2);
        memcpy(&m_packet_motion.m_carMotionData[i].m_worldRightDirY, &m_buffer[index + 32 + (i * 60)], 2);
        memcpy(&m_packet_motion.m_carMotionData[i].m_worldRightDirZ, &m_buffer[index + 34 + (i * 60)], 2);
        memcpy(&m_packet_motion.m_carMotionData[i].m_gForceLateral, &m_buffer[index + 36 + (i * 60)], 4);
        memcpy(&m_packet_motion.m_carMotionData[i].m_gForceLongitudinal, &m_buffer[index + 40 + (i * 60)], 4);
        memcpy(&m_packet_motion.m_carMotionData[i].m_gForceVertical, &m_buffer[index + 44 + (i * 60)], 4);
        memcpy(&m_packet_motion.m_carMotionData[i].m_yaw, &m_buffer[index + 48 + (i * 60)], 4);
        memcpy(&m_packet_motion.m_carMotionData[i].m_pitch, &m_buffer[index + 52 + (i * 60)], 4);
        memcpy(&m_packet_motion.m_carMotionData[i].m_roll, &m_buffer[index + 56 + (i * 60)], 4);
    }

    // finish m_buffer at index 1360
    for(int i = 0; i < 4; i++)
    {
        memcpy(&m_packet_motion.m_suspensionPosition[i], &m_buffer[index + 1344 + (i * 4)], 4);
    }

    // finish m_buffer at index 1376
    for(int i = 0; i < 4; i++)
    {
        memcpy(&m_packet_motion.m_suspensionVelocity[i], &m_buffer[index + 1360 + (i * 4)], 4);
    }

    // finish m_buffer at index 1536
    for(int i = 0; i < 4; i++)
    {
        memcpy(&m_packet_motion.m_suspensionAcceleration[i], &m_buffer[index + 1376 + (i * 4)], 4);
    }

    // finish m_buffer at index 1580
    for(int i = 0; i < 4; i++)
    {
        memcpy(&m_packet_motion.m_wheelSpeed[i], &m_buffer[index + 1392 + (i * 4)], 4);
    }

    // finish m_buffer at index 260
    for(int i = 0; i < 4; i++)
    {
        memcpy(&m_packet_motion.m_wheelSlip[i], &m_buffer[index + 1408 + (i * 4)], 4);
    }

    memcpy(&m_packet_motion.m_localVelocityX, &m_buffer[index + 1424], 4);
    memcpy(&m_packet_motion.m_localVelocityY, &m_buffer[index + 1428], 4);
    memcpy(&m_packet_motion.m_localVelocityZ, &m_buffer[index + 1432], 4);
    memcpy(&m_packet_motion.m_angularVelocityX, &m_buffer[index + 1436], 4);
    memcpy(&m_packet_motion.m_angularVelocityY, &m_buffer[index + 1440], 4);
    memcpy(&m_packet_motion.m_angularVelocityZ, &m_buffer[index + 1444], 4);
    memcpy(&m_packet_motion.m_angularAccelerationX, &m_buffer[index + 1448], 4);
    memcpy(&m_packet_motion.m_angularAccelerationY, &m_buffer[index + 1452], 4);
    memcpy(&m_packet_motion.m_angularAccelerationZ, &m_buffer[index + 1456], 4);
    memcpy(&m_packet_motion.m_frontWheelsAngle, &m_buffer[index + 1460], 4);

    if(debug_packet)
    {
        for(int i = 0; i < 22; i++)
        {
            std::cout << "######## CAR " << i << " ########" << std::endl;
            std::cout << "World position X:" << m_packet_motion.m_carMotionData[i].m_worldPositionX << std::endl;
            std::cout << "World position Y:" << m_packet_motion.m_carMotionData[i].m_worldPositionY << std::endl;
            std::cout << "World position Z:" << m_packet_motion.m_carMotionData[i].m_worldPositionZ << std::endl;
            std::cout << "World velocity X:" << m_packet_motion.m_carMotionData[i].m_worldVelocityX << std::endl;
            std::cout << "World velocity Y:" << m_packet_motion.m_carMotionData[i].m_worldVelocityY << std::endl;
            std::cout << "World velocity Z:" << m_packet_motion.m_carMotionData[i].m_worldVelocityZ << std::endl;
            std::cout << "World forward direction X:" << m_packet_motion.m_carMotionData[i].m_worldForwardDirX << std::endl;
            std::cout << "World forward direction Y:" << m_packet_motion.m_carMotionData[i].m_worldForwardDirY << std::endl;
            std::cout << "World forward direction Z:" << m_packet_motion.m_carMotionData[i].m_worldForwardDirZ << std::endl;
            std::cout << "World right direction X:" << m_packet_motion.m_carMotionData[i].m_worldRightDirX << std::endl;
            std::cout << "World right direction Y:" << m_packet_motion.m_carMotionData[i].m_worldRightDirY << std::endl;
            std::cout << "World right direction Z:" << m_packet_motion.m_carMotionData[i].m_worldRightDirZ << std::endl;
            std::cout << "G-force lateral:" << m_packet_motion.m_carMotionData[i].m_gForceLateral << std::endl;
            std::cout << "G-force longitudinal:" << m_packet_motion.m_carMotionData[i].m_gForceLongitudinal << std::endl;
            std::cout << "G-force vertical:" << m_packet_motion.m_carMotionData[i].m_gForceVertical << std::endl;
            std::cout << "Yaw:" << m_packet_motion.m_carMotionData[i].m_yaw << std::endl;
            std::cout << "Pitch:" << m_packet_motion.m_carMotionData[i].m_pitch << std::endl;
            std::cout << "Roll:" << m_packet_motion.m_carMotionData[i].m_roll << std::endl;
        }

        for(int i = 0; i < 4; i++){
            std::cout << "Suspension position" << m_packet_motion.m_suspensionPosition[i] << std::endl;
        }
        for(int i = 0; i < 4; i++){
            std::cout << "Suspension velocity:" << m_packet_motion.m_suspensionVelocity[i] << std::endl;
        }
        for(int i = 0; i < 4; i++){
            std::cout << "Suspension acceleration:" << m_packet_motion.m_suspensionAcceleration[i] << std::endl;
        }
        for(int i = 0; i < 4; i++){
            std::cout << "Wheel speed:" << m_packet_motion.m_wheelSpeed[i] << std::endl;
        }
        for(int i = 0; i < 4; i++){
            std::cout << "Wheel slip:" << m_packet_motion.m_wheelSlip[i] << std::endl;
        }

        std::cout << "Local velocity X:" << m_packet_motion.m_localVelocityX << std::endl;
        std::cout << "Local velocity Y:" << m_packet_motion.m_localVelocityY << std::endl;
        std::cout << "Local velocity Z:" << m_packet_motion.m_localVelocityZ << std::endl;
        std::cout << "Angular velocity X:" << m_packet_motion.m_angularVelocityX << std::endl;
        std::cout << "Angular velocity Y:" << m_packet_motion.m_angularVelocityY << std::endl;
        std::cout << "Angular velocity Z:" << m_packet_motion.m_angularVelocityZ << std::endl;
        std::cout << "Angular acceleration X:" << m_packet_motion.m_angularAccelerationX << std::endl;
        std::cout << "Angular acceleration Y:" << m_packet_motion.m_angularAccelerationY << std::endl;
        std::cout << "Angular acceleration Z:" << m_packet_motion.m_angularAccelerationZ << std::endl;
        std::cout << "Front wheel angle:" << m_packet_motion.m_frontWheelsAngle << std::endl;

    }

    return 0;
}


int FO_MQTT_Parser::parse_session_data() {

    if(m_size < 632){return -1;}

    int index = 24;

    memcpy(&m_packet_session.m_weather, &m_buffer[index], 1);
    memcpy(&m_packet_session.m_trackTemperature, &m_buffer[index + 1], 1);
    memcpy(&m_packet_session.m_airTemperature, &m_buffer[index + 2], 1);
    memcpy(&m_packet_session.m_totalLaps, &m_buffer[index + 3], 1);
    memcpy(&m_packet_session.m_trackLength, &m_buffer[index + 4], 2);
    memcpy(&m_packet_session.m_sessionType, &m_buffer[index + 6], 1);
    memcpy(&m_packet_session.m_trackId, &m_buffer[index + 7], 1);
    memcpy(&m_packet_session.m_formula, &m_buffer[index + 8], 1);
    memcpy(&m_packet_session.m_sessionTimeLeft, &m_buffer[index + 9], 2);
    memcpy(&m_packet_session.m_sessionDuration, &m_buffer[index + 11], 2);
    memcpy(&m_packet_session.m_pitSpeedLimit, &m_buffer[index + 13], 1);
    memcpy(&m_packet_session.m_gamePaused, &m_buffer[index + 14], 1);
    memcpy(&m_packet_session.m_isSpectating, &m_buffer[index + 15], 1);
    memcpy(&m_packet_session.m_spectatorCarIndex, &m_buffer[index + 16], 1);
    memcpy(&m_packet_session.m_sliProNativeSupport, &m_buffer[index + 17], 1);
    memcpy(&m_packet_session.m_numMarshalZones, &m_buffer[index + 18], 1);

    //105
    for(int i = 0; i < 21; i++){
        memcpy(&m_packet_session.m_marshalZones[i].m_zoneStart, &m_buffer[index + 19 + (i * 5)], 4);
        memcpy(&m_packet_session.m_marshalZones[i].m_zoneFlag, &m_buffer[index + 23 + (i * 5)], 1);
    }

    memcpy(&m_packet_session.m_safetyCarStatus, &m_buffer[index + 124], 1);
    memcpy(&m_packet_session.m_networkGame, &m_buffer[index + 125], 1);
    memcpy(&m_packet_session.m_numWeatherForecastSamples, &m_buffer[index + 126], 1);

    //448
    for(int i = 0; i < 56; i++){
        memcpy(&m_packet_session.m_weatherForecastSamples[i].m_sessionType, &m_buffer[index + 127 + (i * 8)], 1);
        memcpy(&m_packet_session.m_weatherForecastSamples[i].m_timeOffset, &m_buffer[index + 128 + (i * 8)], 1);
        memcpy(&m_packet_session.m_weatherForecastSamples[i].m_weather, &m_buffer[index + 129 + (i * 8)], 1);
        memcpy(&m_packet_session.m_weatherForecastSamples[i].m_trackTemperature, &m_buffer[index + 130 + (i * 8)], 1);
        memcpy(&m_packet_session.m_weatherForecastSamples[i].m_trackTemperatureChange, &m_buffer[index + 131 + (i * 8)], 1);
        memcpy(&m_packet_session.m_weatherForecastSamples[i].m_airTemperature, &m_buffer[index + 132 + (i * 8)], 1);
        memcpy(&m_packet_session.m_weatherForecastSamples[i].m_airTemperatureChange, &m_buffer[index + 133 + (i * 8)], 1);
        memcpy(&m_packet_session.m_weatherForecastSamples[i].m_rainPercentage, &m_buffer[index + 134 + (i * 8)], 1);
    }

    memcpy(&m_packet_session.m_forecastAccuracy, &m_buffer[index + 575], 1);
    memcpy(&m_packet_session.m_aiDifficulty, &m_buffer[index + 576], 1);
    memcpy(&m_packet_session.m_seasonLinkIdentifier, &m_buffer[index + 577], 4);
    memcpy(&m_packet_session.m_weekendLinkIdentifier, &m_buffer[index + 581], 4);
    memcpy(&m_packet_session.m_sessionLinkIdentifier, &m_buffer[index + 585], 4);
    memcpy(&m_packet_session.m_pitStopWindowIdealLap, &m_buffer[index + 589], 1);
    memcpy(&m_packet_session.m_pitStopWindowLatestLap, &m_buffer[index + 590], 1);
    memcpy(&m_packet_session.m_pitStopRejoinPosition, &m_buffer[index + 591], 1);
    memcpy(&m_packet_session.m_steeringAssist, &m_buffer[index + 592], 1);
    memcpy(&m_packet_session.m_brakingAssist, &m_buffer[index + 593], 1);
    memcpy(&m_packet_session.m_gearboxAssist, &m_buffer[index + 594], 1);
    memcpy(&m_packet_session.m_pitAssist, &m_buffer[index + 595], 1);
    memcpy(&m_packet_session.m_pitReleaseAssist, &m_buffer[index + 596], 1);
    memcpy(&m_packet_session.m_ERSAssist, &m_buffer[index + 597], 1);
    memcpy(&m_packet_session.m_DRSAssist, &m_buffer[index + 598], 1);
    memcpy(&m_packet_session.m_dynamicRacingLine, &m_buffer[index + 599], 1);
    memcpy(&m_packet_session.m_dynamicRacingLineType, &m_buffer[index + 600], 1);
    memcpy(&m_packet_session.m_gameMode, &m_buffer[index + 601], 1);
    memcpy(&m_packet_session.m_ruleSet, &m_buffer[index + 602], 1);
    memcpy(&m_packet_session.m_timeOfDay, &m_buffer[index + 603], 4);
    memcpy(&m_packet_session.m_sessionLength, &m_buffer[index + 607], 1);


    if(debug_packet){
        std::cout << "Session weather: " << m_packet_session.m_weather << std::endl;
        std::cout << "Session track temperature: " << m_packet_session.m_trackTemperature << std::endl;
        std::cout << "Session air temperature: " << m_packet_session.m_airTemperature << std::endl;
        std::cout << "Session total laps: " << m_packet_session.m_totalLaps << std::endl;
        std::cout << "Session track length: " << m_packet_session.m_trackLength << std::endl;
        std::cout << "Session session type: " << m_packet_session.m_sessionType << std::endl;
        std::cout << "Session track ID: " << m_packet_session.m_trackId << std::endl;
        std::cout << "Session formula: " << m_packet_session.m_formula << std::endl;
        std::cout << "Session time left: " << m_packet_session.m_sessionTimeLeft << std::endl;
        std::cout << "Session duration: " << m_packet_session.m_sessionDuration << std::endl;
        std::cout << "Session pit speed limit: " << m_packet_session.m_pitSpeedLimit << std::endl;
        std::cout << "Session game paused: " << m_packet_session.m_gamePaused << std::endl;
        std::cout << "Session is_spectating: " << m_packet_session.m_isSpectating << std::endl;
        std::cout << "Session spectator car index: " << m_packet_session.m_spectatorCarIndex << std::endl;
        std::cout << "Session sli-pro native support: " << m_packet_session.m_sliProNativeSupport << std::endl;
        std::cout << "Session number of marshall zones: " << m_packet_session.m_numMarshalZones << std::endl;

        // Print array of marshall zones
        for(int i = 0; i < 21; i++){
            std::cout << "######## Marshall zone " << i << " ########" << std::endl;

            std::cout << "Session marshall zone-start: " << m_packet_session.m_marshalZones[i].m_zoneStart << std::endl;
            std::cout << "Session marshall zone-flag: " << m_packet_session.m_marshalZones[i].m_zoneFlag << std::endl;
        }

        std::cout << "Safety car status: " << m_packet_session.m_safetyCarStatus << std::endl;
        std::cout << "Online?: " << m_packet_session.m_networkGame << std::endl;
        std::cout << "Number of weather samples: " << m_packet_session.m_numWeatherForecastSamples << std::endl;


        // Print array of WeatherForecastSamples
        for(int i = 0; i < 56; i++){
            std::cout << "######## Weather sample " << i << " ########" << std::endl;
            std::cout << "session type: " << m_packet_session.m_weatherForecastSamples[i].m_sessionType << std::endl;
            std::cout << "time-offset: " << m_packet_session.m_weatherForecastSamples[i].m_timeOffset << std::endl;
            std::cout << "weather: " << m_packet_session.m_weatherForecastSamples[i].m_weather << std::endl;
            std::cout << "track temperature: " << m_packet_session.m_weatherForecastSamples[i].m_trackTemperature << std::endl;
            std::cout << "track temperature change: " << m_packet_session.m_weatherForecastSamples[i].m_trackTemperatureChange << std::endl;
            std::cout << "air temperature: " << m_packet_session.m_weatherForecastSamples[i].m_airTemperature << std::endl;
            std::cout << "air temperature change: " << m_packet_session.m_weatherForecastSamples[i].m_airTemperatureChange << std::endl;
            std::cout << "rain percentage: " << m_packet_session.m_weatherForecastSamples[i].m_rainPercentage << std::endl;
        }

        std::cout << "Session forecast accuracy: " << m_packet_session.m_forecastAccuracy << std::endl;
        std::cout << "Session AI difficulty: " << m_packet_session.m_aiDifficulty << std::endl;
        std::cout << "Session season link identifier: " << m_packet_session.m_seasonLinkIdentifier << std::endl;
        std::cout << "Session weekend link identifier: " << m_packet_session.m_weekendLinkIdentifier << std::endl;
        std::cout << "Session session link identifier: " << m_packet_session.m_sessionLinkIdentifier << std::endl;
        std::cout << "Session pit-stop window ideal lap: " << m_packet_session.m_pitStopWindowIdealLap << std::endl;
        std::cout << "Session pit-stop window latest lap: " << m_packet_session.m_pitStopWindowLatestLap << std::endl;
        std::cout << "Session pit-stop rejoin position: " << m_packet_session.m_pitStopRejoinPosition << std::endl;
        std::cout << "Session steering assist: " << m_packet_session.m_steeringAssist << std::endl;
        std::cout << "Session braking assist: " << m_packet_session.m_brakingAssist << std::endl;
        std::cout << "Session gearbox assist: " << m_packet_session.m_gearboxAssist << std::endl;
        std::cout << "Session pit assist: " << m_packet_session.m_pitAssist << std::endl;
        std::cout << "Session pit-release assist: " << m_packet_session.m_pitReleaseAssist << std::endl;
        std::cout << "Session ERS assist: " << m_packet_session.m_ERSAssist << std::endl;
        std::cout << "Session DRS assist: " << m_packet_session.m_DRSAssist << std::endl;
        std::cout << "Session dynamic racing line: " << m_packet_session.m_dynamicRacingLine << std::endl;
        std::cout << "Session dynamic racing line type: " << m_packet_session.m_dynamicRacingLineType << std::endl;
        std::cout << "Session game mode id: " << m_packet_session.m_gameMode << std::endl;
        std::cout << "Session ruleset: " << m_packet_session.m_ruleSet << std::endl;
        std::cout << "Session time min since midnight: " << m_packet_session.m_timeOfDay << std::endl;
        std::cout << "Session length: " << m_packet_session.m_sessionLength << std::endl;

    }

    return 0;
}

int FO_MQTT_Parser::parse_lap_data() {

    if(m_size < 972){return -1;}

    int index = 24;

    for(int i = 0; i < 22; i++){
        memcpy(&m_packet_lap.m_lapData[i].m_lastLapTimeInMS, &m_buffer[index + (i * 43)], 4);
        memcpy(&m_packet_lap.m_lapData[i].m_currentLapTimeInMS, &m_buffer[index + 4 + (i * 43)], 4);
        memcpy(&m_packet_lap.m_lapData[i].m_sector1TimeInMS, &m_buffer[index + 8 + (i * 43)], 2);
        memcpy(&m_packet_lap.m_lapData[i].m_sector2TimeInMS, &m_buffer[index + 10 + (i * 43)], 2);
        memcpy(&m_packet_lap.m_lapData[i].m_lapDistance, &m_buffer[index + 12 + (i * 43)], 4);
        memcpy(&m_packet_lap.m_lapData[i].m_totalDistance, &m_buffer[index + 16 + (i * 43)], 4);
        memcpy(&m_packet_lap.m_lapData[i].m_safetyCarDelta, &m_buffer[index + 20 + (i * 43)], 4);
        memcpy(&m_packet_lap.m_lapData[i].m_carPosition, &m_buffer[index + 24 + (i * 43)], 1);
        memcpy(&m_packet_lap.m_lapData[i].m_currentLapNum, &m_buffer[index + 25 + (i * 43)], 1);
        memcpy(&m_packet_lap.m_lapData[i].m_pitStatus, &m_buffer[index + 26 + (i * 43)], 1);
        memcpy(&m_packet_lap.m_lapData[i].m_numPitStops, &m_buffer[index + 27 + (i * 43)], 1);
        memcpy(&m_packet_lap.m_lapData[i].m_sector, &m_buffer[index + 28 + (i * 43)], 1);
        memcpy(&m_packet_lap.m_lapData[i].m_currentLapInvalid, &m_buffer[index + 29 + (i * 43)], 1);
        memcpy(&m_packet_lap.m_lapData[i].m_penalties, &m_buffer[index + 30 + (i * 43)], 1);
        memcpy(&m_packet_lap.m_lapData[i].m_warnings, &m_buffer[index + 31 + (i * 43)], 1);
        memcpy(&m_packet_lap.m_lapData[i].m_numUnservedDriveThroughPens, &m_buffer[index + 32 + (i * 43)], 1);
        memcpy(&m_packet_lap.m_lapData[i].m_numUnservedStopGoPens, &m_buffer[index + 33 + (i * 43)], 1);
        memcpy(&m_packet_lap.m_lapData[i].m_gridPosition, &m_buffer[index + 34 + (i * 43)], 1);
        memcpy(&m_packet_lap.m_lapData[i].m_driverStatus, &m_buffer[index + 35 + (i * 43)], 1);
        memcpy(&m_packet_lap.m_lapData[i].m_resultStatus, &m_buffer[index + 36 + (i * 43)], 1);
        memcpy(&m_packet_lap.m_lapData[i].m_pitLaneTimerActive, &m_buffer[index + 37 + (i * 43)], 1);
        memcpy(&m_packet_lap.m_lapData[i].m_pitLaneTimeInLaneInMS, &m_buffer[index + 38 + (i * 43)], 2);
        memcpy(&m_packet_lap.m_lapData[i].m_pitStopTimerInMS, &m_buffer[index + 40 + (i * 43)], 2);
        memcpy(&m_packet_lap.m_lapData[i].m_pitStopShouldServePen, &m_buffer[index + 42 + (i * 43)], 1);
    }

    memcpy(&m_packet_lap.m_timeTrialPBCarIdx, &m_buffer[970], 1);
    memcpy(&m_packet_lap.m_timeTrialRivalCarIdx, &m_buffer[971], 1);

    if(debug_packet){
        for(int i = 0; i < 22; i++){
            std::cout << "######## CAR " << i << " ########" << std::endl;
            std::cout << "Last lap in ms: " << m_packet_lap.m_lapData[i].m_lastLapTimeInMS << std::endl;
            std::cout << "Current lap in ms: " << m_packet_lap.m_lapData[i].m_currentLapTimeInMS << std::endl;
            std::cout << "Sector 1 time in ms: " << m_packet_lap.m_lapData[i].m_sector1TimeInMS << std::endl;
            std::cout << "Sector 2 time in ms: " << m_packet_lap.m_lapData[i].m_sector2TimeInMS << std::endl;
            std::cout << "Total lap distance in M: " << m_packet_lap.m_lapData[i].m_lapDistance << std::endl;
            std::cout << "Total race distance in M: " << m_packet_lap.m_lapData[i].m_totalDistance << std::endl;
            std::cout << "Delta safety car in sec: " << m_packet_lap.m_lapData[i].m_safetyCarDelta << std::endl;
            std::cout << "Car race position: " << m_packet_lap.m_lapData[i].m_carPosition << std::endl;
            std::cout << "Current lap number: " << m_packet_lap.m_lapData[i].m_currentLapNum << std::endl;
            std::cout << "Pit status: " << m_packet_lap.m_lapData[i].m_pitStatus << std::endl;
            std::cout << "Number of pitstops: " << m_packet_lap.m_lapData[i].m_numPitStops << std::endl;
            std::cout << "Current sector: " << m_packet_lap.m_lapData[i].m_sector << std::endl;
            std::cout << "Is lap valid?: " << m_packet_lap.m_lapData[i].m_currentLapInvalid << std::endl;
            std::cout << "Penalties in sec: " << m_packet_lap.m_lapData[i].m_penalties << std::endl;
            std::cout << "Number of warnings: " << m_packet_lap.m_lapData[i].m_warnings << std::endl;
            std::cout << "Drive through penalties: " << m_packet_lap.m_lapData[i].m_numUnservedDriveThroughPens << std::endl;
            std::cout << "Stop go penalties: " << m_packet_lap.m_lapData[i].m_numUnservedStopGoPens << std::endl;
            std::cout << "Started grid position: " << m_packet_lap.m_lapData[i].m_gridPosition << std::endl;
            std::cout << "Driver status: " << m_packet_lap.m_lapData[i].m_driverStatus << std::endl;
            std::cout << "Results status: " << m_packet_lap.m_lapData[i].m_resultStatus << std::endl;
            std::cout << "Pit lane timer active: " << m_packet_lap.m_lapData[i].m_pitLaneTimerActive << std::endl;
            std::cout << "Pit lane time in ms: " << m_packet_lap.m_lapData[i].m_pitLaneTimeInLaneInMS << std::endl;
            std::cout << "Pit lane stop time in ms: " << m_packet_lap.m_lapData[i].m_pitStopTimerInMS << std::endl;
            std::cout << "Should serve penalty during pit: " << m_packet_lap.m_lapData[i].m_pitStopShouldServePen << std::endl;
        }
    }

    return 0;
}

void FO_MQTT_Parser::initialize_event_data() {
    m_string_codes["SSTA"] = SSTA;
    m_string_codes["SEND"] = SEND;
    m_string_codes["FTLP"] = FTLP;
    m_string_codes["RTMT"] = RTMT;
    m_string_codes["DRSE"] = DRSE;
    m_string_codes["DRSD"] = DRSD;
    m_string_codes["TMPT"] = TMPT;
    m_string_codes["CHQF"] = CHQF;
    m_string_codes["RCWN"] = RCWN;
    m_string_codes["PENA"] = PENA;
    m_string_codes["STPT"] = SPTP;
    m_string_codes["STLG"] = STLG;
    m_string_codes["LGOT"] = LGOT;
    m_string_codes["DTSV"] = DTSV;
    m_string_codes["SGSV"] = SGSV;
    m_string_codes["FLBK"] = FLBK;
    m_string_codes["BUTN"] = BUTN;

}

int FO_MQTT_Parser::parse_event_data() {
    if(m_size < 40){return -1;}

    int index = 24;

    for(int i = 0; i < 4; i++){
        memcpy(&m_packet_event.m_eventStringCode[i], &m_buffer[index + i], 1);
    }
    std::string code((char*)m_packet_event.m_eventStringCode);
    switch(m_string_codes[code]){
        case SSTA:{

            break;
        }
        case SEND:{

            break;
        }
        case FTLP:{
            memcpy(&m_packet_event.m_eventDetails.FastestLap.vehicleIdx, &m_buffer[index+4], 1);
            memcpy(&m_packet_event.m_eventDetails.FastestLap.lapTime, &m_buffer[index+5], 4);

            break;
        }
        case RTMT:{
            memcpy(&m_packet_event.m_eventDetails.Retirement.vehicleIdx, &m_buffer[index+4], 1);

            break;
        }
        case DRSE:{

            break;
        }
        case DRSD:{

            break;
        }
        case TMPT:{
            memcpy(&m_packet_event.m_eventDetails.TeamMateInPits.vehicleIdx, &m_buffer[index+4], 1);
            break;
        }
        case CHQF:{

            break;
        }
        case RCWN:{
            memcpy(&m_packet_event.m_eventDetails.RaceWinner.vehicleIdx, &m_buffer[index+4], 1);
            break;
        }
        case PENA:{
            memcpy(&m_packet_event.m_eventDetails.Penalty.penaltyType, &m_buffer[index+4], 1);
            memcpy(&m_packet_event.m_eventDetails.Penalty.infringementType, &m_buffer[index+5], 1);
            memcpy(&m_packet_event.m_eventDetails.Penalty.vehicleIdx, &m_buffer[index+6], 1);
            memcpy(&m_packet_event.m_eventDetails.Penalty.otherVehicleIdx, &m_buffer[index+7], 1);
            memcpy(&m_packet_event.m_eventDetails.Penalty.time, &m_buffer[index+8], 1);
            memcpy(&m_packet_event.m_eventDetails.Penalty.lapNum, &m_buffer[index+9], 1);
            memcpy(&m_packet_event.m_eventDetails.Penalty.placesGained, &m_buffer[index+10], 1);
            break;
        }
        case SPTP:{
            memcpy(&m_packet_event.m_eventDetails.SpeedTrap.vehicleIdx, &m_buffer[index+4], 1);
            memcpy(&m_packet_event.m_eventDetails.SpeedTrap.speed, &m_buffer[index+5], 4);
            memcpy(&m_packet_event.m_eventDetails.SpeedTrap.isOverallFastestInSession, &m_buffer[index+9], 1);
            memcpy(&m_packet_event.m_eventDetails.SpeedTrap.isDriverFastestInSession, &m_buffer[index+10], 1);
            memcpy(&m_packet_event.m_eventDetails.SpeedTrap.fastestVehicleIdxInSession, &m_buffer[index+11], 1);
            memcpy(&m_packet_event.m_eventDetails.SpeedTrap.fastestSpeedInSession, &m_buffer[index+12], 4);
            break;
        }
        case STLG:{
            memcpy(&m_packet_event.m_eventDetails.StartLIghts.numLights, &m_buffer[index+4], 1);
            break;
        }
        case LGOT:{

            break;
        }
        case DTSV:{
            memcpy(&m_packet_event.m_eventDetails.DriveThroughPenaltyServed.vehicleIdx, &m_buffer[index+4], 1);
            break;
        }
        case SGSV:{
            memcpy(&m_packet_event.m_eventDetails.StopGoPenaltyServed.vehicleIdx, &m_buffer[index+4], 1);
            break;
        }
        case FLBK:{
            memcpy(&m_packet_event.m_eventDetails.Flashback.flashbackFrameIdentifier, &m_buffer[index+4], 4);
            memcpy(&m_packet_event.m_eventDetails.Flashback.flashbackSessionTime, &m_buffer[index+8], 4);
            break;
        }
        case BUTN:{
            memcpy(&m_packet_event.m_eventDetails.Buttons.m_buttonStatus, &m_buffer[index+4], 4);
            break;
        }
    }

    return 0;
}



int FO_MQTT_Parser::parse_participant_data() {
    if(m_size < 1257){return -1;}

    int index = 24;

    memcpy(&m_packet_participants.m_numActiveCars, &m_buffer[index++], 1);// 1 in front of the 22 packets

    for(int i = 0; i < 22; i++){
        memcpy(&m_packet_participants.m_participants[i].m_aiControlled, &m_buffer[index+(i*56)], 1);
        memcpy(&m_packet_participants.m_participants[i].m_driverId, &m_buffer[index+1+(i*56)], 1);
        memcpy(&m_packet_participants.m_participants[i].m_networkId, &m_buffer[index+2+(i*56)], 1);
        memcpy(&m_packet_participants.m_participants[i].m_teamId, &m_buffer[index+3+(i*56)], 1);
        memcpy(&m_packet_participants.m_participants[i].m_myTeam, &m_buffer[index+4+(i*56)], 1);
        memcpy(&m_packet_participants.m_participants[i].m_raceNumber, &m_buffer[index+5+(i*56)], 1);
        memcpy(&m_packet_participants.m_participants[i].m_nationality, &m_buffer[index+6+(i*56)], 1);
        memcpy(&m_packet_participants.m_participants[i].m_name, &m_buffer[index+7+(i*56)], 48);
        memcpy(&m_packet_participants.m_participants[i].m_yourTelemetry, &m_buffer[index+55+(i*56)], 1);
    }

    if(debug_packet){
        for(int i = 0; i < 22; i++){
            std::cout << "######## CAR " << i << " ########" << std::endl;
            std::cout << "Is an AI?: " << m_packet_participants.m_participants[i].m_aiControlled << std::endl;
            std::cout << "Driver ID: " << m_packet_participants.m_participants[i].m_aiControlled << std::endl;
            std::cout << "Network ID: " << m_packet_participants.m_participants[i].m_aiControlled << std::endl;
            std::cout << "Team ID: " << m_packet_participants.m_participants[i].m_aiControlled << std::endl;
            std::cout << "My Team?: " << m_packet_participants.m_participants[i].m_aiControlled << std::endl;
            std::cout << "Race number: " << m_packet_participants.m_participants[i].m_aiControlled << std::endl;
            std::cout << "Nationality: " << m_packet_participants.m_participants[i].m_aiControlled << std::endl;
            std::cout << "Name: " << m_packet_participants.m_participants[i].m_aiControlled << std::endl;
            std::cout << "Telemetry settings: " << m_packet_participants.m_participants[i].m_aiControlled << std::endl;
        }
    }

    return 0;
}

int FO_MQTT_Parser::parse_car_setups() {
    if(m_size < 1102){return -1;}

    int index = 24;

    for(int i = 0; i < 22; i++){
        memcpy(&m_packet_car_setup.m_carSetups[i].m_frontWing, &m_buffer[index + (i*49)], 1);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_rearWing, &m_buffer[index + 1 + (i*49)], 1);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_onThrottle, &m_buffer[index + 2 + (i*49)], 1);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_offThrottle, &m_buffer[index + 3 + (i*49)], 1);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_frontCamber, &m_buffer[index + 4 + (i*49)], 4);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_rearCamber, &m_buffer[index + 8 + (i*49)], 4);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_frontToe, &m_buffer[index + 12 + (i*49)], 4);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_rearToe, &m_buffer[index + 16 + (i*49)], 4);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_frontSuspension, &m_buffer[index + 20 + (i*49)], 1);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_rearSuspension, &m_buffer[index + 21 + (i*49)], 1);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_frontAntiRollBar, &m_buffer[index + 22 + (i*49)], 1);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_rearAntiRollBar, &m_buffer[index + 23 + (i*49)], 1);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_frontSuspensionHeight, &m_buffer[index + 24 + (i*49)], 1);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_rearSuspensionHeight, &m_buffer[index + 25 + (i*49)], 1);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_brakePressure, &m_buffer[index + 26 + (i*49)], 1);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_brakeBias, &m_buffer[index + 27 + (i*49)], 1);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_rearLeftTyrePressure, &m_buffer[index + 28 + (i*49)], 4);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_rearRightTyrePressure, &m_buffer[index + 32 + (i*49)], 4);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_frontLeftTyrePressure, &m_buffer[index + 36 + (i*49)], 4);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_frontRightTyrePressure, &m_buffer[index + 40 + (i*49)], 4);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_ballast, &m_buffer[index + 44 + (i*49)], 1);
        memcpy(&m_packet_car_setup.m_carSetups[i].m_fuelLoad, &m_buffer[index + 45 + (i*49)], 4);
    }

    return 0;
}

int FO_MQTT_Parser::parse_car_telemetry() {

    if(m_size < 1347){return -1;}

    int index = 24;

    for(int i = 0; i < 22; i++)
    {
        memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_speed, &m_buffer[index+(i*60)],2);
        memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_throttle, &m_buffer[index+2+(i*60)],4);
        memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_steer, &m_buffer[index+6+(i*60)],4);
        memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_brake, &m_buffer[index+10+(i*60)],4);
        memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_clutch, &m_buffer[index+14+(i*60)],1);
        memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_gear, &m_buffer[index+15+(i*60)],1);
        memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_engineRPM, &m_buffer[index+16+(i*60)],2);
        memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_drs, &m_buffer[index+18+(i*60)],1);
        memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_revLightsPercent, &m_buffer[index+19+(i*60)],1);
        memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_revLightsBitValue, &m_buffer[index+20+(i*60)],2);
        for(int j = 0; j < 4; j++){
            memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_brakesTemperature[j], &m_buffer[index+22+(j*2)+(i*60)],2);
        }
        for(int j = 0; j < 4; j++){
            memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_tyresSurfaceTemperature[j], &m_buffer[index+30+j+(i*60)],1);
        }
        for(int j = 0; j < 4; j++){
            memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_tyresInnerTemperature[j], &m_buffer[index+34+j+(i*60)],1);
        }
        memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_engineTemperature, &m_buffer[index+38],2);
        for(int j = 0; j < 4; j++){
            memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_tyresPressure[j], &m_buffer[index+40+(j*4)+(i*60)],4);
        }
        for(int j = 0; j < 4; j++){
            memcpy(&m_packet_car_telemetry.m_carTelemetryData[i].m_surfaceType[j], &m_buffer[index+56+j+(i*60)],1);
        }
    }
    memcpy(&m_packet_car_telemetry.m_mfdPanelIndex, &m_buffer[1344],1);
    memcpy(&m_packet_car_telemetry.m_mfdPanelIndexSecondaryPlayer, &m_buffer[1345],1);
    memcpy(&m_packet_car_telemetry.m_suggestedGear, &m_buffer[1346],1);

    if(debug_packet == false){
        for(int i = 0; i < 1; i++) {

            std::cout << "######## CAR " << i << " ########" << std::endl;

            std::cout << "Packet speed:" << m_packet_car_telemetry.m_carTelemetryData[i].m_speed << std::endl;

            std::cout << "Packet throttle:" << m_packet_car_telemetry.m_carTelemetryData[i].m_throttle << std::endl;

            std::cout << "Packet steer:" << m_packet_car_telemetry.m_carTelemetryData[i].m_steer << std::endl;

            std::cout << "Packet brake:" << m_packet_car_telemetry.m_carTelemetryData[i].m_brake << std::endl;

            std::cout << "Packet clutch:" << (int)m_packet_car_telemetry.m_carTelemetryData[i].m_clutch << std::endl;

            std::cout << "Packet gear:" << (int)m_packet_car_telemetry.m_carTelemetryData[i].m_gear << std::endl;

            std::cout << "Packet engine RPM:" << m_packet_car_telemetry.m_carTelemetryData[i].m_engineRPM << std::endl;

            std::cout << "Packet DRS:" << (int)m_packet_car_telemetry.m_carTelemetryData[i].m_drs << std::endl;

            std::cout << "Packet revLightsPercentage:" << (int)m_packet_car_telemetry.m_carTelemetryData[i].m_revLightsPercent
                      << std::endl;

            std::cout << "Packet m_revLightsBitValue:" << m_packet_car_telemetry.m_carTelemetryData[i].m_revLightsBitValue
                      << std::endl;

            for (int j = 0; j < 4; j++) {
                std::cout << "Packet brakes temperature:"
                          << m_packet_car_telemetry.m_carTelemetryData[i].m_brakesTemperature[j] << std::endl;
            }

            for (int j = 0; j < 4; j++) {
                std::cout << "Packet tyres surface temperature:"
                          << (int)m_packet_car_telemetry.m_carTelemetryData[i].m_tyresSurfaceTemperature[j] << std::endl;
            }

            for (int j = 0; j < 4; j++) {
                std::cout << "Packet tyres inner temperature:"
                          << (int)m_packet_car_telemetry.m_carTelemetryData[i].m_tyresInnerTemperature[j] << std::endl;
            }

            std::cout << "Packet engine temperature:" << m_packet_car_telemetry.m_carTelemetryData[i].m_engineTemperature
                      << std::endl;

            for (int j = 0; j < 4; j++) {
                std::cout << "Packet tyre pressure:" << m_packet_car_telemetry.m_carTelemetryData[i].m_tyresPressure[j]
                          << std::endl;
            }

            for (int j = 0; j < 4; j++) {
                std::cout << "Packet surface type:" << (int)m_packet_car_telemetry.m_carTelemetryData[i].m_surfaceType[j]
                          << std::endl;
            }
        }
    }



    return 0;
}

int FO_MQTT_Parser::parse_car_status() {

    // Check the size of received buffer
    if(m_size < 1058){return -1;}

    int index = 24;

    for(int i=0; i < 22; i++)
    {
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_tractionControl,&m_buffer[index+(i*47)],1);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_antiLockBrakes,&m_buffer[index+1+(i*47)],1);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_fuelMix,&m_buffer[index+2+(i*47)],1);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_frontBrakeBias,&m_buffer[index+3+(i*47)],1);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_pitLimiterStatus,&m_buffer[index+4+(i*47)],1);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_fuelInTank,&m_buffer[index+5+(i*47)],4);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_fuelCapacity,&m_buffer[index+9+(i*47)],4);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_fuelRemainingLaps,&m_buffer[index+13+(i*47)],4);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_maxRPM,&m_buffer[index+17+(i*47)],2);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_idleRPM,&m_buffer[index+19+(i*47)],2);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_maxGears,&m_buffer[index+21+(i*47)],1);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_drsAllowed,&m_buffer[index+22+(i*47)],1);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_drsActivationDistance,&m_buffer[index+23+(i*47)],2);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_actualTyreCompound,&m_buffer[index+25+(i*47)],1);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_visualTyreCompound,&m_buffer[index+26+(i*47)],1);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_tyresAgeLaps,&m_buffer[index+27+(i*47)],1);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_vehicleFiaFlags,&m_buffer[index+28+(i*47)],1);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_ersStoreEnergy,&m_buffer[index+29+(i*47)],4);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_ersDeployMode,&m_buffer[index+33+(i*47)],1);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_ersHarvestedThisLapMGUK,&m_buffer[index+34+(i*47)],4);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_ersHarvestedThisLapMGUH,&m_buffer[index+38+(i*47)],4);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_ersDeployedThisLap,&m_buffer[index+42+(i*47)],4);
        std::memcpy(&m_packet_car_status.m_carStatusData[i].m_networkPaused,&m_buffer[index+46+(i*47)],1);
    }

    if(debug_packet){
        for (int i = 0; i < 22; i++) {
            std::cout << "######## CAR " << i << " ########" << std::endl;
            std::cout << "Traction control:" << m_packet_car_status.m_carStatusData[i].m_tractionControl << std::endl;
            std::cout << "Anti-lock brakes:" << m_packet_car_status.m_carStatusData[i].m_antiLockBrakes << std::endl;
            std::cout << "Fuel mix:" << m_packet_car_status.m_carStatusData[i].m_fuelMix << std::endl;
            std::cout << "Front brake-bias:" << m_packet_car_status.m_carStatusData[i].m_frontBrakeBias << std::endl;
            std::cout << "Pit limiter status:" << m_packet_car_status.m_carStatusData[i].m_pitLimiterStatus << std::endl;
            std::cout << "Fuel in tank:" << m_packet_car_status.m_carStatusData[i].m_fuelInTank << std::endl;
            std::cout << "Fuel capacity:" << m_packet_car_status.m_carStatusData[i].m_fuelCapacity << std::endl;
            std::cout << "Fuel remaining-laps:" << m_packet_car_status.m_carStatusData[i].m_fuelRemainingLaps << std::endl;
            std::cout << "Max RPM:" << m_packet_car_status.m_carStatusData[i].m_maxRPM << std::endl;
            std::cout << "Max idle RPM:" << m_packet_car_status.m_carStatusData[i].m_idleRPM << std::endl;
            std::cout << "Max gears:" << m_packet_car_status.m_carStatusData[i].m_maxGears << std::endl;
            std::cout << "DRS allowed:" << m_packet_car_status.m_carStatusData[i].m_drsAllowed << std::endl;
            std::cout << "DRS activation distance:" << m_packet_car_status.m_carStatusData[i].m_drsActivationDistance << std::endl;
            std::cout << "Actual tyre compound:" << m_packet_car_status.m_carStatusData[i].m_actualTyreCompound << std::endl;
            std::cout << "Visual tyre compound:" << m_packet_car_status.m_carStatusData[i].m_visualTyreCompound << std::endl;
            std::cout << "Tyre age laps:" << m_packet_car_status.m_carStatusData[i].m_tyresAgeLaps << std::endl;
            std::cout << "Vehicle FIA flags:" << m_packet_car_status.m_carStatusData[i].m_vehicleFiaFlags << std::endl;
            std::cout << "ERS store energy:" << m_packet_car_status.m_carStatusData[i].m_ersStoreEnergy << std::endl;
            std::cout << "ERS deploy mode:" << m_packet_car_status.m_carStatusData[i].m_ersDeployMode << std::endl;
            std::cout << "ERS harvested this lap MGU-K:" << m_packet_car_status.m_carStatusData[i].m_ersHarvestedThisLapMGUK << std::endl;
            std::cout << "ERS harvested this lap MGU-H:" << m_packet_car_status.m_carStatusData[i].m_ersHarvestedThisLapMGUH << std::endl;
            std::cout << "ERS deployed this lap:" << m_packet_car_status.m_carStatusData[i].m_ersDeployedThisLap << std::endl;
            std::cout << "Network paused:" << m_packet_car_status.m_carStatusData[i].m_networkPaused << std::endl;
        }
    }

    return 0;
}

int FO_MQTT_Parser::parse_final_classification_data() {

    // Check the size of the buffer
    if(m_size < 1015){return -1;}

    int index = 24;

    std::memcpy(&m_packet_final_classification.m_numCars, &m_buffer[index++], 1);

    for (int i = 0; i < 22; i++) {
        std::memcpy(&m_packet_final_classification.m_classificationData[i].m_position, &m_buffer[index + (i * 45)], 1);
        std::memcpy(&m_packet_final_classification.m_classificationData[i].m_numLaps, &m_buffer[index + 1 + (i * 45)], 1);
        std::memcpy(&m_packet_final_classification.m_classificationData[i].m_gridPosition, &m_buffer[index + 2 + (i * 45)], 1);
        std::memcpy(&m_packet_final_classification.m_classificationData[i].m_points, &m_buffer[index + 3 + (i * 45)], 1);
        std::memcpy(&m_packet_final_classification.m_classificationData[i].m_numPitStops, &m_buffer[index + 4 + (i * 45)], 1);
        std::memcpy(&m_packet_final_classification.m_classificationData[i].m_resultStatus, &m_buffer[index + 5 + (i * 45)], 1);
        std::memcpy(&m_packet_final_classification.m_classificationData[i].m_bestLapTimeInMS, &m_buffer[index + 6 + (i * 45)], 4);
        std::memcpy(&m_packet_final_classification.m_classificationData[i].m_totalRaceTime, &m_buffer[index + 10 + (i * 45)], 8);
        std::memcpy(&m_packet_final_classification.m_classificationData[i].m_penaltiesTime, &m_buffer[index + 18 + (i * 45)], 1);
        std::memcpy(&m_packet_final_classification.m_classificationData[i].m_numPenalties, &m_buffer[index + 19 + (i * 45)], 1);
        std::memcpy(&m_packet_final_classification.m_classificationData[i].m_numTyreStints, &m_buffer[index + 20 + (i * 45)], 1);

        // For loop 8 times
        for (int j = 0; j < 8; j++) {
            std::memcpy(&m_packet_final_classification.m_classificationData[i].m_tyreStintsActual[j], &m_buffer[index + 21 + j + (i * 45)], 1);
        }

        // For loop 8 times
        for (int j = 0; j < 8; j++) {
            std::memcpy(&m_packet_final_classification.m_classificationData[i].m_tyreStintsVisual[j], &m_buffer[index + 29 + j + (i * 45)], 1);
        }

        // For loop 8 times
        for (int j = 0; j < 8; j++) {
            std::memcpy(&m_packet_final_classification.m_classificationData[i].m_tyreStintsEndLaps[j], &m_buffer[index + 37 + j + (i * 45)], 1);
        }
    }

    if (debug_packet)
    {
        std::cout << "Number of cars:" << m_packet_final_classification.m_numCars << std::endl;

        for (int i = 0; i < 22; i++) {
            std::cout << "######## CAR " << i << " ########" << std::endl;
            std::cout << "Finishing position:" << m_packet_final_classification.m_classificationData[i].m_position << std::endl;
            std::cout << "Number of laps completed:" << m_packet_final_classification.m_classificationData[i].m_position << std::endl;
            std::cout << "Grid position:" << m_packet_final_classification.m_classificationData[i].m_position << std::endl;
            std::cout << "Points scored:" << m_packet_final_classification.m_classificationData[i].m_position << std::endl;
            std::cout << "Number of pit stops made:" << m_packet_final_classification.m_classificationData[i].m_position << std::endl;
            std::cout << "Result status:" << m_packet_final_classification.m_classificationData[i].m_position << std::endl;
            std::cout << "Best lap tim in ms:" << m_packet_final_classification.m_classificationData[i].m_position << std::endl;
            std::cout << "Total race time:" << m_packet_final_classification.m_classificationData[i].m_position << std::endl;
            std::cout << "Total penalty time:" << m_packet_final_classification.m_classificationData[i].m_position << std::endl;
            std::cout << "Number of penalties:" << m_packet_final_classification.m_classificationData[i].m_position << std::endl;
            std::cout << "Number of tyre stints:" << m_packet_final_classification.m_classificationData[i].m_position << std::endl;

            for (int j = 0; j < 8; j++) {
                std::cout << "Tyre stint actual [" << j << "]:" << m_packet_final_classification.m_classificationData[i].m_tyreStintsActual[j] << std::endl;
            }

            for (int j = 0; j < 8; j++) {
                std::cout << "Tyre stint visual [" << j << "]:" << m_packet_final_classification.m_classificationData[i].m_tyreStintsVisual[j] << std::endl;
            }

            for (int j = 0; j < 8; j++) {
                std::cout << "Tyre stint end laps [" << j << "]:" << m_packet_final_classification.m_classificationData[i].m_tyreStintsEndLaps[j] << std::endl;
            }
        }
    }

    return 0;
}

int FO_MQTT_Parser::parse_lobby_info() {

    if(m_size < 1191){return -1;}

    int index = 24;

    std::memcpy(&m_packet_lobby_info.m_numPlayers, &m_buffer[index++], 1);

    for(int i = 0; i < 22; i++){
        std::memcpy(&m_packet_lobby_info.m_lobbyPlayers[i].m_aiControlled, &m_buffer[index + (i * 53)], 1);
        std::memcpy(&m_packet_lobby_info.m_lobbyPlayers[i].m_teamId, &m_buffer[index + 1 + (i * 53)], 1);
        std::memcpy(&m_packet_lobby_info.m_lobbyPlayers[i].m_nationality, &m_buffer[index + 2 + (i * 53)], 1);
        std::memcpy(&m_packet_lobby_info.m_lobbyPlayers[i].m_name, &m_buffer[index + 3 + (i * 53)], 48);
        std::memcpy(&m_packet_lobby_info.m_lobbyPlayers[i].m_carNumber, &m_buffer[index + 51 + (i * 53)], 1);
        std::memcpy(&m_packet_lobby_info.m_lobbyPlayers[i].m_readyStatus, &m_buffer[index + 52 + (i * 53)], 1);
    }

    if (debug_packet){

        std::cout << "Number of players: " << m_packet_lobby_info.m_numPlayers << std::endl;

        for(int i = 0; i < 22; i++){
            std::cout << "######## CAR " << i << " ########" << std::endl;
            std::cout << "Is AI: " << m_packet_lobby_info.m_lobbyPlayers[i].m_aiControlled << std::endl;
            std::cout << "Team id: " << m_packet_lobby_info.m_lobbyPlayers[i].m_teamId << std::endl;
            std::cout << "Nationality: " << m_packet_lobby_info.m_lobbyPlayers[i].m_nationality << std::endl;
            std::cout << "Name: " << m_packet_lobby_info.m_lobbyPlayers[i].m_name << std::endl;
            std::cout << "Car number: " << m_packet_lobby_info.m_lobbyPlayers[i].m_carNumber << std::endl;
            std::cout << "Ready status: " << m_packet_lobby_info.m_lobbyPlayers[i].m_readyStatus << std::endl;
        }

    }

    return 0;
}

int FO_MQTT_Parser::parse_car_damage() {
    if(m_size < 948){return -1;}

    int index = 24;

    for(int i = 0; i < 22; i++){
        for(int j = 0; j < 4; j++){
            std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_tyresWear, &m_buffer[index + (j*4) + (i*42)], 4);
        }

        for(int j = 0; j < 4; j++){
            std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_tyresDamage, &m_buffer[index + 16 + (j) + (i*42)], 1);
        }

        for(int j = 0; j < 4; j++){
            std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_brakesDamage, &m_buffer[index + 20 + (j) + (i*42)], 1);
        }
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_frontLeftWingDamage, &m_buffer[index + 24 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_frontRightWingDamage, &m_buffer[index + 25 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_rearWingDamage, &m_buffer[index + 28 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_floorDamage, &m_buffer[index + 27 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_diffuserDamage, &m_buffer[index + 28 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_sidepodDamage, &m_buffer[index + 29 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_drsFault, &m_buffer[index + 30 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_ersFault, &m_buffer[index + 31 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_gearBoxDamage, &m_buffer[index + 32 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_engineDamage, &m_buffer[index + 33 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_engineMGUHWear, &m_buffer[index + 34 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_engineESWear, &m_buffer[index + 35 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_engineCEWear, &m_buffer[index + 36 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_engineICEWear, &m_buffer[index + 37 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_engineMGUKWear, &m_buffer[index + 38 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_engineTCWear, &m_buffer[index + 39 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_engineBlown, &m_buffer[index + 40 + (i*42)], 1);
        std::memcpy(&m_packet_car_damage.m_carDamageData[i].m_engineSeized, &m_buffer[index + 41 + (i*42)], 1);
    }

    if (debug_packet) {
        for (int i = 0; i < 22; i++) {
            std::cout << "######## CAR " << i << " ########" << std::endl;

            for(int j = 0; j < 4; j++){
                std::cout << "Tyre wear " << j << " : " << m_packet_car_damage.m_carDamageData[i].m_tyresWear[j] << std::endl;
            }
            for(int j = 0; j < 4; j++){
                std::cout << "Tyre damage " << j << " : " << m_packet_car_damage.m_carDamageData[i].m_tyresDamage[j] << std::endl;
            }
            for(int j = 0; j < 4; j++){
                std::cout << "Breaks damage " << j << " : " << m_packet_car_damage.m_carDamageData[i].m_brakesDamage[j] << std::endl;
            }

            std::cout << "Front wing left damage: " << m_packet_car_damage.m_carDamageData[i].m_frontLeftWingDamage << std::endl;
            std::cout << "Front wing right damage: " << m_packet_car_damage.m_carDamageData[i].m_frontRightWingDamage << std::endl;
            std::cout << "Rear wing damage: " << m_packet_car_damage.m_carDamageData[i].m_rearWingDamage << std::endl;
            std::cout << "Floor damage: " << m_packet_car_damage.m_carDamageData[i].m_floorDamage << std::endl;
            std::cout << "Diffuser damage: " << m_packet_car_damage.m_carDamageData[i].m_diffuserDamage << std::endl;
            std::cout << "Side-pod damage: " << m_packet_car_damage.m_carDamageData[i].m_sidepodDamage << std::endl;
            std::cout << "DRS fault: " << m_packet_car_damage.m_carDamageData[i].m_drsFault << std::endl;
            std::cout << "ERS fault: " << m_packet_car_damage.m_carDamageData[i].m_ersFault << std::endl;
            std::cout << "Gearbox damage: " << m_packet_car_damage.m_carDamageData[i].m_gearBoxDamage << std::endl;
            std::cout << "Engine damage: " << m_packet_car_damage.m_carDamageData[i].m_engineDamage << std::endl;
            std::cout << "MGUH Wear: " << m_packet_car_damage.m_carDamageData[i].m_engineMGUHWear << std::endl;
            std::cout << "ES wear: " << m_packet_car_damage.m_carDamageData[i].m_engineESWear << std::endl;
            std::cout << "CE wear: " << m_packet_car_damage.m_carDamageData[i].m_engineCEWear << std::endl;
            std::cout << "ICE wear: " << m_packet_car_damage.m_carDamageData[i].m_engineICEWear << std::endl;
            std::cout << "MGUK wear: " << m_packet_car_damage.m_carDamageData[i].m_engineMGUKWear << std::endl;
            std::cout << "TC wear: " << m_packet_car_damage.m_carDamageData[i].m_engineTCWear << std::endl;
            std::cout << "Engine blown: " << m_packet_car_damage.m_carDamageData[i].m_engineBlown << std::endl;
            std::cout << "Engine seized: " << m_packet_car_damage.m_carDamageData[i].m_engineSeized << std::endl;
        }
    }

    return 0;
}

int FO_MQTT_Parser::parse_session_history() {

    if(m_size < 1155){return -1;}

    int index = 24;

    std::memcpy(&m_packet_session_history.m_carIdx, &m_buffer[index], 1);
    std::memcpy(&m_packet_session_history.m_numLaps, &m_buffer[index+ 1], 1);
    std::memcpy(&m_packet_session_history.m_numTyreStints, &m_buffer[index+ 2], 1);
    std::memcpy(&m_packet_session_history.m_bestLapTimeLapNum, &m_buffer[index+ 3], 1);
    std::memcpy(&m_packet_session_history.m_bestSector1LapNum, &m_buffer[index+ 4], 1);
    std::memcpy(&m_packet_session_history.m_bestSector2LapNum, &m_buffer[index+ 5], 1);
    std::memcpy(&m_packet_session_history.m_bestSector3LapNum, &m_buffer[index+ 6], 1);

    for(int i = 0; i < 100; i++){
        std::memcpy(&m_packet_session_history.m_lapHistoryData[i].m_lapTimeInMS, &m_buffer[index + 7 + (i*11)], 4);
        std::memcpy(&m_packet_session_history.m_lapHistoryData[i].m_sector1TimeInMS, &m_buffer[index + 11 + (i*11)], 2);
        std::memcpy(&m_packet_session_history.m_lapHistoryData[i].m_sector2TimeInMS, &m_buffer[index + 13 + (i*11)], 2);
        std::memcpy(&m_packet_session_history.m_lapHistoryData[i].m_sector3TimeInMS, &m_buffer[index + 15 + (i*11)], 2);
        std::memcpy(&m_packet_session_history.m_lapHistoryData[i].m_lapValidBitFlags, &m_buffer[index + 17 + (i*11)], 1);
    }

    for(int i = 0; i < 8; i++){
        std::memcpy(&m_packet_session_history.m_tyreStintsHistoryData[i].m_endLap, &m_buffer[index + 1131 + (i*3)], 1);
        std::memcpy(&m_packet_session_history.m_tyreStintsHistoryData[i].m_tyreActualCompound, &m_buffer[index + 1132 + (i*3)], 1);
        std::memcpy(&m_packet_session_history.m_tyreStintsHistoryData[i].m_tyreVisualCompound, &m_buffer[index + 1133 + (i*3)], 1);

    }

    if(debug_packet){
        std::cout << "Car index: " << m_packet_session_history.m_carIdx << std::endl;
        std::cout << "Number of laps: " << m_packet_session_history.m_numLaps << std::endl;
        std::cout << "Number of tyre stints: " << m_packet_session_history.m_numTyreStints << std::endl;
        std::cout << "Best lap done on lap: " << m_packet_session_history.m_bestLapTimeLapNum << std::endl;
        std::cout << "Best sector 1 done on lap: " << m_packet_session_history.m_bestSector1LapNum << std::endl;
        std::cout << "Best sector 2 done on lap: " << m_packet_session_history.m_bestSector2LapNum << std::endl;
        std::cout << "Best sector 3 done on lap: " << m_packet_session_history.m_bestSector3LapNum << std::endl;

        for(int i = 0; i < 100; i++){
            std::cout << "######## LAP " << i << " ########" << std::endl;
            std::cout << "Lap time: " << m_packet_session_history.m_lapHistoryData[i].m_lapTimeInMS << std::endl;
            std::cout << "Sector 1: " << m_packet_session_history.m_lapHistoryData[i].m_sector1TimeInMS << std::endl;
            std::cout << "Sector 2: " << m_packet_session_history.m_lapHistoryData[i].m_sector2TimeInMS << std::endl;
            std::cout << "Sector 3: " << m_packet_session_history.m_lapHistoryData[i].m_sector3TimeInMS << std::endl;
            std::cout << "Valid: " << m_packet_session_history.m_lapHistoryData[i].m_lapValidBitFlags << std::endl;
        }

        for(int i = 0; i < 8; i++){
            std::cout << "######## STINT " << i << " ########" << std::endl;
            std::cout << "Lap tyre usage ended: " << m_packet_session_history.m_tyreStintsHistoryData[i].m_endLap << std::endl;
            std::cout << "Actual compound: " << m_packet_session_history.m_tyreStintsHistoryData[i].m_tyreActualCompound << std::endl;
            std::cout << "Visual compound: " << m_packet_session_history.m_tyreStintsHistoryData[i].m_tyreVisualCompound << std::endl;

        }
    }

    return 0;
}