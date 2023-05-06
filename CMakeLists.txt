cmake_minimum_required(VERSION 3.25)
project(FO_Parser)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(FO_Parser
        include/F122_structs.h
        include/FO_MQTT_Parser.h
        src/FO_MQTT_Parser.cpp
        main.cpp)
