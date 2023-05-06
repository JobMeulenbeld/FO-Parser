//
// Created by Arief on 10/03/2023.
//

#ifndef F1TELEMETRY_F1_MQTT_PARSER_H
#define F1TELEMETRY_F1_MQTT_PARSER_H

#include "F122_structs.h"
#include <iostream>
#include <cstring>
#include <map>

#define NUM

class FO_MQTT_Parser {
public:
    FO_MQTT_Parser(std::string host, std::string client_id);

private:

    int parse(char* buffer, unsigned long size);

    enum string_code{
        SSTA,
        SEND,
        FTLP,
        RTMT,
        DRSE,
        DRSD,
        TMPT,
        CHQF,
        RCWN,
        PENA,
        SPTP,
        STLG,
        LGOT,
        DTSV,
        SGSV,
        FLBK,
        BUTN
    };
    
    int parse_packet_header();
    int parse_motion_data();
    int parse_session_data();
    int parse_lap_data();
    int parse_event_data();
    void initialize_event_data();
    int parse_participant_data();
    int parse_car_setups();
    int parse_car_telemetry();
    int parse_car_status();
    int parse_final_classification_data();
    int parse_lobby_info();
    int parse_car_damage();
    int parse_session_history();

    char* m_buffer;
    int m_size;


    PacketHeader m_packetheader;
    bool debug_packet = false;
    PacketMotionData m_packet_motion;
    PacketSessionData m_packet_session;
    PacketLapData m_packet_lap;
    PacketEventData m_packet_event;
    PacketParticipantsData m_packet_participants;
    PacketCarSetupData m_packet_car_setup;
    PacketCarTelemetryData m_packet_car_telemetry;
    PacketCarStatusData m_packet_car_status;
    PacketFinalClassificationData m_packet_final_classification;
    PacketLobbyInfoData m_packet_lobby_info;
    PacketCarDamageData m_packet_car_damage;
    PacketSessionHistoryData m_packet_session_history;

    std::map<std::string, string_code> m_string_codes;
};


#endif //F1TELEMETRY_F1_MQTT_PARSER_H
