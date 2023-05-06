//
// Created by meule on 3/10/2023.
//

#ifndef F1TELEMETRY_F122_STRUCTS_H
#define F1TELEMETRY_F122_STRUCTS_H

#endif //F1TELEMETRY_F122_STRUCTS_H

#include <cstdint>
#include <stdlib.h>

struct PacketHeader
{
    unsigned short m_packetFormat; // 2022
    unsigned char m_gameMajorVersion; // Game major version - "X.00"
    unsigned char m_gameMinorVersion; // Game minor version - "1.XX"
    unsigned char m_packetVersion; // Version of this packet type, all start from 1
    unsigned char m_packetId; // Identifier for the packet type, see below
    unsigned long m_sessionUID; // Unique identifier for the session
    float m_sessionTime; // Session timestamp
    unsigned int m_frameIdentifier; // Identifier for the frame the data was retrieved on
    unsigned char m_playerCarIndex; // Index of player'm_socket car in the array
    unsigned char m_secondaryPlayerCarIndex; // Index of secondary player'm_socket car in the array (splitscreen)
                                             // 255 if no second player
};

/* Motion Packet
 *  The motion packet gives physics data for all the cars being driven. There is additional data for the car
 *  being driven with the goal of being able to drive a motion platform setup.
 *  N.B. For the normalised vectors below, to convert to float values divide by 32767.0f – 16-bit signed values
 *  are used to pack the data and on the assumption that direction values are always between -1.0f and 1.0f.
 *  Frequency: Rate as specified in menus
 *  Size: 1464 bytes
 *  Version: 1
 */

// 60
struct CarMotionData
{
    float m_worldPositionX; // World space X position
    float m_worldPositionY; // World space Y position
    float m_worldPositionZ; // World space Z position
    float m_worldVelocityX; // Velocity in world space X
    float m_worldVelocityY; // Velocity in world space Y
    float m_worldVelocityZ; // Velocity in world space Z
    short m_worldForwardDirX; // World space forward X direction (normalised)
    short m_worldForwardDirY; // World space forward Y direction (normalised)
    short m_worldForwardDirZ; // World space forward Z direction (normalised)
    short m_worldRightDirX; // World space right X direction (normalised)
    short m_worldRightDirY; // World space right Y direction (normalised)
    short m_worldRightDirZ; // World space right Z direction (normalised)
    float m_gForceLateral; // Lateral G-Force component
    float m_gForceLongitudinal; // Longitudinal G-Force component
    float m_gForceVertical; // Vertical G-Force component
    float m_yaw; // Yaw angle in radians
    float m_pitch; // Pitch angle in radians
    float m_roll; // Roll angle in radians
};

struct PacketMotionData
{
    PacketHeader m_header; // Header
    CarMotionData m_carMotionData[22]; // Data for all cars on track // Extra player car ONLY data
    float m_suspensionPosition[4]; // Note: All wheel arrays have the following order:
    float m_suspensionVelocity[4]; // RL, RR, FL, FR
    float m_suspensionAcceleration[4]; // RL, RR, FL, FR
    float m_wheelSpeed[4]; // Speed of each wheel
    float m_wheelSlip[4]; // Slip ratio for each wheel
    float m_localVelocityX; // Velocity in local space
    float m_localVelocityY; // Velocity in local space
    float m_localVelocityZ; // Velocity in local space
    float m_angularVelocityX; // Angular velocity x-component
    float m_angularVelocityY; // Angular velocity y-component
    float m_angularVelocityZ; // Angular velocity z-component
    float m_angularAccelerationX; // Angular velocity x-component
    float m_angularAccelerationY; // Angular velocity y-component
    float m_angularAccelerationZ; // Angular velocity z-component
    float m_frontWheelsAngle; // Current front wheels angle in radians
};

/* Session Packet
 * The session packet includes details about the current session in progress.
 * Frequency: 2 per second
 * Size: 632 bytes
 * Version: 1
 */

// 5 bytes
struct MarshalZone
{
    float m_zoneStart; // Fraction (0..1) of way through the lap the marshal zone starts
    char m_zoneFlag; // -1 = invalid/unknown, 0 = none, 1 = green, 2 = blue, 3 = yellow, 4 = red
};

// 8 bytes
struct WeatherForecastSample
{
    unsigned char m_sessionType; // 0 = unknown, 1 = P1, 2 = P2, 3 = P3, 4 = Short P, 5 = Q1, 6 = Q2, 7 = Q3, 8 = Short Q, 9 = OSQ, 10 = R, 11 = R2, 12 = R3, 13 = Time Trial
    unsigned char m_timeOffset; // Time in minutes the forecast is for
    unsigned char m_weather; // Weather - 0 = clear, 1 = light cloud, 2 = overcast, 3 = light rain, 4 = heavy rain, 5 = storm
    char m_trackTemperature; // Track temp. in degrees Celsius
    char m_trackTemperatureChange; // Track temp. change – 0 = up, 1 = down, 2 = no change
    char m_airTemperature; // Air temp. in degrees celsius
    char m_airTemperatureChange; // Air temp. change – 0 = up, 1 = down, 2 = no change
    unsigned char m_rainPercentage; // Rain percentage (0-100)
};

// short = 2
// char = 1
// int = 4
// float = 4
// long = 8

struct PacketSessionData
{
    PacketHeader m_header; // Header // 24
    unsigned char m_weather; // Weather - 0 = clear, 1 = light cloud, 2 = overcast, 3 = light rain, 4 = heavy rain, 5 = storm
    char m_trackTemperature; // Track temp. in degrees celsius
    char m_airTemperature; // Air temp. in degrees celsius
    unsigned char m_totalLaps; // Total number of laps in this race
    unsigned short m_trackLength; // Track length in metres
    unsigned char m_sessionType; // 0 = unknown, 1 = P1, 2 = P2, 3 = P3, 4 = Short P, 5 = Q1, 6 = Q2, 7 = Q3, 8 = Short Q, 9 = OSQ 10 = R, 11 = R2, 12 = R3, 13 = Time Trial
    char m_trackId; // -1 for unknown, see appendix
    unsigned char m_formula; // Formula, 0 = F1 Modern, 1 = F1 Classic, 2 = F2, 3 = F1 Generic, 4 = Beta, 5 = Supercars, 6 = Esports, 7 = F2 2021
    unsigned short m_sessionTimeLeft; // Time left in session in seconds
    unsigned short m_sessionDuration; // Session duration in seconds
    unsigned char m_pitSpeedLimit; // Pit speed limit in kilometres per hour
    unsigned char m_gamePaused; // Whether the game is paused – network game only
    unsigned char m_isSpectating; // Whether the player is spectating
    unsigned char m_spectatorCarIndex; // Index of the car being spectated
    unsigned char m_sliProNativeSupport; // SLI Pro support, 0 = inactive, 1 = active
    unsigned char m_numMarshalZones; // Number of marshal zones to follow // 43
    MarshalZone m_marshalZones[21]; // List of marshal zones – max 21 // 5 * 21 = 105 bytes // 148
    unsigned char m_safetyCarStatus; // 0 = no safety car, 1 = full 2 = virtual, 3 = formation lap
    unsigned char m_networkGame; // 0 = offline, 1 = online
    unsigned char m_numWeatherForecastSamples; // Number of weather samples to follow // 151
    WeatherForecastSample m_weatherForecastSamples[56]; // Array of weather forecast samples // 151 + (8 * 56) = 556
    unsigned char m_forecastAccuracy; // 0 = Perfect, 1 = Approximate
    unsigned char m_aiDifficulty; // AI Difficulty rating – 0-110
    unsigned int m_seasonLinkIdentifier; // Identifier for season - persists across saves
    unsigned int m_weekendLinkIdentifier; // Identifier for weekend - persists across saves
    unsigned int m_sessionLinkIdentifier; // Identifier for session - persists across saves
    unsigned char m_pitStopWindowIdealLap; // Ideal lap to pit on for current strategy (player)
    unsigned char m_pitStopWindowLatestLap; // Latest lap to pit on for current strategy (player)
    unsigned char m_pitStopRejoinPosition; // Predicted position to rejoin at (player)
    unsigned char m_steeringAssist; // 0 = off, 1 = on
    unsigned char m_brakingAssist; // 0 = off, 1 = low, 2 = medium, 3 = high
    unsigned char m_gearboxAssist; // 1 = manual, 2 = manual & suggested gear, 3 = auto
    unsigned char m_pitAssist; // 0 = off, 1 = on
    unsigned char m_pitReleaseAssist; // 0 = off, 1 = on
    unsigned char m_ERSAssist; // 0 = off, 1 = on
    unsigned char m_DRSAssist; // 0 = off, 1 = on // 580
    unsigned char m_dynamicRacingLine; // 0 = off, 1 = corners only, 2 = full
    unsigned char m_dynamicRacingLineType; // 0 = 2D, 1 = 3D
    unsigned char m_gameMode; // Game mode id - see appendix
    unsigned char m_ruleSet; // Ruleset - see appendix // 627
    unsigned int m_timeOfDay; // Local time of day - minutes since midnight
    unsigned char m_sessionLength; // 0 = None, 2 = Very Short, 3 = Short, 4 = Medium 5 = Medium Long, 6 = Long, 7 = Full // 589
};

/* Lap Data Packet
 * The lap data packet gives details of all the cars in the session.
 * Frequency: Rate as specified in menus
 * Size: 972 bytes
 * Version: 1
 */

//43 bytes
struct LapData
{
    unsigned int m_lastLapTimeInMS; // Last lap time in milliseconds
    unsigned int m_currentLapTimeInMS; // Current time around the lap in milliseconds
    unsigned short m_sector1TimeInMS; // Sector 1 time in milliseconds
    unsigned short m_sector2TimeInMS; // Sector 2 time in milliseconds
    float m_lapDistance; // Distance vehicle is around current lap in metres – could
// be negative if line hasn’t been crossed yet
    float m_totalDistance; // Total distance travelled in session in metres – could
// be negative if line hasn’t been crossed yet
    float m_safetyCarDelta; // Delta in seconds for safety car
    unsigned char m_carPosition; // Car race position
    unsigned char m_currentLapNum; // Current lap number
    unsigned char m_pitStatus; // 0 = none, 1 = pitting, 2 = in pit area
    unsigned char m_numPitStops; // Number of pit stops taken in this race
    unsigned char m_sector; // 0 = sector1, 1 = sector2, 2 = sector3
    unsigned char m_currentLapInvalid; // Current lap invalid - 0 = valid, 1 = invalid
    unsigned char m_penalties; // Accumulated time penalties in seconds to be added
    unsigned char m_warnings; // Accumulated number of warnings issued
    unsigned char m_numUnservedDriveThroughPens; // Num drive through pens left to serve
    unsigned char m_numUnservedStopGoPens; // Num stop-go pens left to serve
    unsigned char m_gridPosition; // Grid position the vehicle started the race in
    unsigned char m_driverStatus; // Status of driver - 0 = in garage, 1 = flying lap
// 2 = in lap, 3 = out lap, 4 = on track
    unsigned char m_resultStatus; // Result status - 0 = invalid, 1 = inactive, 2 = active
// 3 = finished, 4 = didnotfinish, 5 = disqualified
// 6 = not classified, 7 = retired
    unsigned char m_pitLaneTimerActive; // Pit lane timing, 0 = inactive, 1 = active
    unsigned short m_pitLaneTimeInLaneInMS; // If active, the current time spent in the pit lane in ms
    unsigned short m_pitStopTimerInMS; // Time of the actual pit stop in ms
    unsigned char m_pitStopShouldServePen; // Whether the car should serve a penalty at this stop
};
struct PacketLapData
{
    PacketHeader m_header; // Header
    LapData m_lapData[22]; // Lap data for all cars on track
    unsigned char m_timeTrialPBCarIdx; // Index of Personal Best car in time trial (255 if invalid)
    unsigned char m_timeTrialRivalCarIdx; // Index of Rival car in time trial (255 if invalid)
};

/* Event Packet
 * This packet gives details of events that happen during the course of a session.
 * Frequency: When the event occurs
 * Size: 40 bytes
 * Version: 1
 */

// The event details packet is different for each type of event.
// Make sure only the correct type is interpreted.
union EventDataDetails
{
    struct
    {
        unsigned char vehicleIdx; // Vehicle index of car achieving fastest lap
        float lapTime; // Lap time is in seconds
    } FastestLap;
    struct
    {
        unsigned char vehicleIdx; // Vehicle index of car retiring
    } Retirement;
    struct
    {
        unsigned char vehicleIdx; // Vehicle index of team mate
    } TeamMateInPits;
    struct
    {
        unsigned char vehicleIdx; // Vehicle index of the race winner
    } RaceWinner;
    struct
    {
        unsigned char penaltyType; // Penalty type – see Appendices
        unsigned char infringementType; // Infringement type – see Appendices
        unsigned char vehicleIdx; // Vehicle index of the car the penalty is applied to
        unsigned char otherVehicleIdx; // Vehicle index of the other car involved
        unsigned char time; // Time gained, or time spent doing action in seconds
        unsigned char lapNum; // Lap the penalty occurred on
        unsigned char placesGained; // Number of places gained by this
    } Penalty;
    struct
    {
        unsigned char vehicleIdx; // Vehicle index of the vehicle triggering speed trap
        float speed; // Top speed achieved in kilometres per hour
        unsigned char isOverallFastestInSession; // Overall fastest speed in session = 1, otherwise 0
        unsigned char isDriverFastestInSession; // Fastest speed for driver in session = 1, otherwise 0
        unsigned char fastestVehicleIdxInSession;// Vehicle index of the vehicle that is the fastest
// in this session
        float fastestSpeedInSession; // Speed of the vehicle that is the fastest
// in this session
    } SpeedTrap;
    struct
    {
        unsigned char numLights; // Number of lights showing
    } StartLIghts;
    struct
    {
        unsigned char vehicleIdx; // Vehicle index of the vehicle serving drive through
    } DriveThroughPenaltyServed;
    struct
    {
        unsigned char vehicleIdx; // Vehicle index of the vehicle serving stop go
    } StopGoPenaltyServed;
    struct
    {
        unsigned int flashbackFrameIdentifier; // Frame identifier flashed back to
        float flashbackSessionTime; // Session time flashed back to
    } Flashback;
    struct
    {
        unsigned int m_buttonStatus; // Bit flags specifying which buttons are being pressed
// currently - see appendices
    } Buttons;
};
struct PacketEventData
{
    PacketHeader m_header; // Header
    unsigned char m_eventStringCode[4]; // Event string code, see below
    EventDataDetails m_eventDetails; // Event details - should be interpreted differently
// for each type
};

// TODO: EVENT STRING CODES

/* Participants Packet
 * This is a list of participants in the race. If the vehicle is controlled by AI, then the name will be the driver
 * name. If this is a multiplayer game, the names will be the Steam Id on PC, or the LAN name if appropriate.
 * N.B. on Xbox One, the names will always be the driver name, on PS4 the name will be the LAN name if
 * playing a LAN game, otherwise it will be the driver name.
 * The array should be indexed by vehicle index.
 * Frequency: Every 5 seconds
 * Size: 1257 bytes
 * Version: 1
 */

struct ParticipantData
{
    unsigned char m_aiControlled; // Whether the vehicle is AI (1) or Human (0) controlled
    unsigned char m_driverId; // Driver id - see appendix, 255 if network human
    unsigned char m_networkId; // Network id – unique identifier for network players
    unsigned char m_teamId; // Team id - see appendix
    unsigned char m_myTeam; // My team flag – 1 = My Team, 0 = otherwise
    unsigned char m_raceNumber; // Race number of the car
    unsigned char m_nationality; // Nationality of the driver
    char m_name[48]; // Name of participant in UTF-8 format – null terminated
// Will be truncated with ... (U+2026) if too long
    unsigned char m_yourTelemetry; // The player'm_socket UDP setting, 0 = restricted, 1 = public
};
struct PacketParticipantsData
{
    PacketHeader m_header; // Header
    unsigned char m_numActiveCars; // Number of active cars in the data – should match number of
// cars on HUD
    ParticipantData m_participants[22];
};

/* Car Setups Packet
This packet details the car setups for each vehicle in the session. Note that in multiplayer games, other
player cars will appear as blank, you will only be able to see your car setup and AI cars.
Frequency: 2 per second
Size: 1102 bytes
Version: 1
*/

struct CarSetupData
{
    unsigned char m_frontWing; // Front wing aero
    unsigned char m_rearWing; // Rear wing aero
    unsigned char m_onThrottle; // Differential adjustment on throttle (percentage)
    unsigned char m_offThrottle; // Differential adjustment off throttle (percentage)
    float m_frontCamber; // Front camber angle (suspension geometry)
    float m_rearCamber; // Rear camber angle (suspension geometry)
    float m_frontToe; // Front toe angle (suspension geometry)
    float m_rearToe; // Rear toe angle (suspension geometry)
    unsigned char m_frontSuspension; // Front suspension
    unsigned char m_rearSuspension; // Rear suspension
    unsigned char m_frontAntiRollBar; // Front anti-roll bar
    unsigned char m_rearAntiRollBar; // Front anti-roll bar
    unsigned char m_frontSuspensionHeight; // Front ride height
    unsigned char m_rearSuspensionHeight; // Rear ride height
    unsigned char m_brakePressure; // Brake pressure (percentage)
    unsigned char m_brakeBias; // Brake bias (percentage)
    float m_rearLeftTyrePressure; // Rear left tyre pressure (PSI)
    float m_rearRightTyrePressure; // Rear right tyre pressure (PSI)
    float m_frontLeftTyrePressure; // Front left tyre pressure (PSI)
    float m_frontRightTyrePressure; // Front right tyre pressure (PSI)
    unsigned char m_ballast; // Ballast
    float m_fuelLoad; // Fuel load
};
struct PacketCarSetupData
{
    PacketHeader m_header; // Header
    CarSetupData m_carSetups[22];
};

/*Car Telemetry Packet
This packet details telemetry for all the cars in the race. It details various values that would be recorded
on the car such as speed, throttle application, DRS etc. Note that the rev light configurations are
presented separately as well and will mimic real life driver preferences.
Frequency: Rate as specified in menus
Size: 1347 bytes
Version: 1
 */

// 60 bytes
struct CarTelemetryData
{
    unsigned short m_speed; // Speed of car in kilometres per hour
    float m_throttle; // Amount of throttle applied (0.0 to 1.0)
    float m_steer; // Steering (-1.0 (full lock left) to 1.0 (full lock right))
    float m_brake; // Amount of brake applied (0.0 to 1.0)
    unsigned char m_clutch; // Amount of clutch applied (0 to 100)
    char m_gear; // Gear selected (1-8, N=0, R=-1)
    unsigned short m_engineRPM; // Engine RPM
    unsigned char m_drs; // 0 = off, 1 = on
    unsigned char m_revLightsPercent; // Rev lights indicator (percentage)
    unsigned short m_revLightsBitValue; // Rev lights (bit 0 = leftmost LED, bit 14 = rightmost LED)
    unsigned short m_brakesTemperature[4]; // Brakes temperature (celsius)
    unsigned char m_tyresSurfaceTemperature[4]; // Tyres surface temperature (celsius)
    unsigned char m_tyresInnerTemperature[4]; // Tyres inner temperature (celsius)
    unsigned short m_engineTemperature; // Engine temperature (celsius)
    float m_tyresPressure[4]; // Tyres pressure (PSI)
    unsigned char m_surfaceType[4]; // Driving surface, see appendices
};
struct PacketCarTelemetryData
{
    PacketHeader m_header; // Header
    CarTelemetryData m_carTelemetryData[22];
    unsigned char m_mfdPanelIndex; // Index of MFD panel open - 255 = MFD closed
// Single player, race – 0 = Car setup, 1 = Pits
// 2 = Damage, 3 = Engine, 4 = Temperatures
// May vary depending on game mode
    unsigned char m_mfdPanelIndexSecondaryPlayer; // See above
    char m_suggestedGear; // Suggested gear for the player (1-8)
// 0 if no gear suggested
};

/* Car Status Packet
This packet details car statuses for all the cars in the race.
Frequency: Rate as specified in menus
Size: 1058 bytes
Version: 1
 */

struct CarStatusData
{
    unsigned char m_tractionControl; // Traction control - 0 = off, 1 = medium, 2 = full
    unsigned char m_antiLockBrakes; // 0 (off) - 1 (on)
    unsigned char m_fuelMix; // Fuel mix - 0 = lean, 1 = standard, 2 = rich, 3 = max
    unsigned char m_frontBrakeBias; // Front brake bias (percentage)
    unsigned char m_pitLimiterStatus; // Pit limiter status - 0 = off, 1 = on
    float m_fuelInTank; // Current fuel mass
    float m_fuelCapacity; // Fuel capacity
    float m_fuelRemainingLaps; // Fuel remaining in terms of laps (value on MFD)
    unsigned short m_maxRPM; // Cars max RPM, point of rev limiter
    unsigned short m_idleRPM; // Cars idle RPM
    unsigned char m_maxGears; // Maximum number of gears
    unsigned char m_drsAllowed; // 0 = not allowed, 1 = allowed
    unsigned short m_drsActivationDistance; // 0 = DRS not available, non-zero - DRS will be available
// in [X] metres
    unsigned char m_actualTyreCompound; // F1 Modern - 16 = C5, 17 = C4, 18 = C3, 19 = C2, 20 = C1
// 7 = inter, 8 = wet
// F1 Classic - 9 = dry, 10 = wet
// F2 – 11 = super soft, 12 = soft, 13 = medium, 14 = hard
// 15 = wet
    unsigned char m_visualTyreCompound; // F1 visual (can be different from actual compound)
// 16 = soft, 17 = medium, 18 = hard, 7 = inter, 8 = wet
// F1 Classic – same as above
// F2 ‘19, 15 = wet, 19 – super soft, 20 = soft
// 21 = medium , 22 = hard
    unsigned char m_tyresAgeLaps; // Age in laps of the current set of tyres
    char m_vehicleFiaFlags; // -1 = invalid/unknown, 0 = none, 1 = green
// 2 = blue, 3 = yellow, 4 = red
    float m_ersStoreEnergy; // ERS energy store in Joules
    unsigned char m_ersDeployMode; // ERS deployment mode, 0 = none, 1 = medium
// 2 = hotlap, 3 = overtake
    float m_ersHarvestedThisLapMGUK; // ERS energy harvested this lap by MGU-K
    float m_ersHarvestedThisLapMGUH; // ERS energy harvested this lap by MGU-H
    float m_ersDeployedThisLap; // ERS energy deployed this lap
    unsigned char m_networkPaused; // Whether the car is paused in a network game
};
struct PacketCarStatusData
{
    PacketHeader m_header; // Header
    CarStatusData m_carStatusData[22];
};

/* Final Classification Packet
This packet details the final classification at the end of the race, and the data will match with the post
race results screen. This is especially useful for multiplayer games where it is not always possible to send
lap times on the final frame because of network delay.
Frequency: Once at the end of a race
Size: 1015 bytes
Version: 1
 */

struct FinalClassificationData
{
    unsigned char m_position; // Finishing position
    unsigned char m_numLaps; // Number of laps completed
    unsigned char m_gridPosition; // Grid position of the car
    unsigned char m_points; // Number of points scored
    unsigned char m_numPitStops; // Number of pit stops made
    unsigned char m_resultStatus; // Result status - 0 = invalid, 1 = inactive, 2 = active
// 3 = finished, 4 = didnotfinish, 5 = disqualified
// 6 = not classified, 7 = retired
    unsigned int m_bestLapTimeInMS; // Best lap time of the session in milliseconds
    double m_totalRaceTime; // Total race time in seconds without penalties
    unsigned char m_penaltiesTime; // Total penalties accumulated in seconds
    unsigned char m_numPenalties; // Number of penalties applied to this driver
    unsigned char m_numTyreStints; // Number of tyres stints up to maximum
    unsigned char m_tyreStintsActual[8]; // Actual tyres used by this driver
    unsigned char m_tyreStintsVisual[8]; // Visual tyres used by this driver
    unsigned char m_tyreStintsEndLaps[8]; // The lap number stints end on
};
struct PacketFinalClassificationData
{
    PacketHeader m_header; // Header
    unsigned char m_numCars; // Number of cars in the final classification
    FinalClassificationData m_classificationData[22];
};

/* Lobby Info Packet
This packet details the players currently in a multiplayer lobby. It details each player’m_socket selected car, any
AI involved in the game and also the ready status of each of the participants.
Frequency: Two every second when in the lobby
Size: 1191 bytes
Version: 1
 */

struct LobbyInfoData
{
    unsigned char m_aiControlled; // Whether the vehicle is AI (1) or Human (0) controlled
    unsigned char m_teamId; // Team id - see appendix (255 if no team currently selected)
    unsigned char m_nationality; // Nationality of the driver
    char m_name[48]; // Name of participant in UTF-8 format – null terminated
// Will be truncated with ... (U+2026) if too long
    unsigned char m_carNumber; // Car number of the player
    unsigned char m_readyStatus; // 0 = not ready, 1 = ready, 2 = spectating
};
struct PacketLobbyInfoData
{
    PacketHeader m_header; // Header
// Packet specific data
    unsigned char m_numPlayers; // Number of players in the lobby data
    LobbyInfoData m_lobbyPlayers[22];
};

/* Car Damage Packet
This packet details car damage parameters for all the cars in the race.
Frequency: 2 per second
Size: 948 bytes
Version: 1
 */

struct CarDamageData
{
    float m_tyresWear[4]; // Tyre wear (percentage)
    unsigned char m_tyresDamage[4]; // Tyre damage (percentage)
    unsigned char m_brakesDamage[4]; // Brakes damage (percentage)
    unsigned char m_frontLeftWingDamage; // Front left wing damage (percentage)
    unsigned char m_frontRightWingDamage; // Front right wing damage (percentage)
    unsigned char m_rearWingDamage; // Rear wing damage (percentage)
    unsigned char m_floorDamage; // Floor damage (percentage)
    unsigned char m_diffuserDamage; // Diffuser damage (percentage)
    unsigned char m_sidepodDamage; // Sidepod damage (percentage)
    unsigned char m_drsFault; // Indicator for DRS fault, 0 = OK, 1 = fault
    unsigned char m_ersFault; // Indicator for ERS fault, 0 = OK, 1 = fault
    unsigned char m_gearBoxDamage; // Gear box damage (percentage)
    unsigned char m_engineDamage; // Engine damage (percentage)
    unsigned char m_engineMGUHWear; // Engine wear MGU-H (percentage)
    unsigned char m_engineESWear; // Engine wear ES (percentage)
    unsigned char m_engineCEWear; // Engine wear CE (percentage)
    unsigned char m_engineICEWear; // Engine wear ICE (percentage)
    unsigned char m_engineMGUKWear; // Engine wear MGU-K (percentage)
    unsigned char m_engineTCWear; // Engine wear TC (percentage)
    unsigned char m_engineBlown; // Engine blown, 0 = OK, 1 = fault
    unsigned char m_engineSeized; // Engine seized, 0 = OK, 1 = fault
};

struct PacketCarDamageData
{
    PacketHeader m_header; // Header
    CarDamageData m_carDamageData[22];
};

/* Session History Packet
This packet contains lap times and tyre usage for the session. This packet works slightly differently
to other packets. To reduce CPU and bandwidth, each packet relates to a specific vehicle and is
sent every 1/20 m_socket, and the vehicle being sent is cycled through. Therefore in a 20 car race you
should receive an update for each vehicle at least once per second.
Note that at the end of the race, after the final classification packet has been sent, a final bulk update
of all the session histories for the vehicles in that session will be sent.
Frequency: 20 per second but cycling through cars
Size: 1155 bytes
Version: 1
 */

struct LapHistoryData
{
    unsigned int m_lapTimeInMS; // Lap time in milliseconds
    unsigned short m_sector1TimeInMS; // Sector 1 time in milliseconds
    unsigned short m_sector2TimeInMS; // Sector 2 time in milliseconds
    unsigned short m_sector3TimeInMS; // Sector 3 time in milliseconds
    unsigned char m_lapValidBitFlags; // 0x01 bit set-lap valid, 0x02 bit set-sector 1 valid
// 0x04 bit set-sector 2 valid, 0x08 bit set-sector 3 valid
};
struct TyreStintHistoryData
{
    unsigned char m_endLap; // Lap the tyre usage ends on (255 of current tyre)
    unsigned char m_tyreActualCompound; // Actual tyres used by this driver
    unsigned char m_tyreVisualCompound; // Visual tyres used by this driver
};
struct PacketSessionHistoryData
{
    PacketHeader m_header; // Header
    unsigned char m_carIdx; // Index of the car this lap data relates to
    unsigned char m_numLaps; // Num laps in the data (including current partial lap)
    unsigned char m_numTyreStints; // Number of tyre stints in the data
    unsigned char m_bestLapTimeLapNum; // Lap the best lap time was achieved on
    unsigned char m_bestSector1LapNum; // Lap the best Sector 1 time was achieved on
    unsigned char m_bestSector2LapNum; // Lap the best Sector 2 time was achieved on
    unsigned char m_bestSector3LapNum; // Lap the best Sector 3 time was achieved on
    LapHistoryData m_lapHistoryData[100]; // 100 laps of data max
    TyreStintHistoryData m_tyreStintsHistoryData[8];
};