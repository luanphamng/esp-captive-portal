#ifndef DATA_MSG_DEF_H
#define DATA_MSG_DEF_H

#define DEBUG_MODE true

#include "DataMsgDefine.h"
struct ControlDataMotor_t
{
    unsigned int isStart = 0;
    uint8_t iSpeed = 0;
    unsigned int iDurationInMin = 0;
    unsigned int iSensitivity = 0;
    unsigned int iAutoSpeed = 0;
    unsigned int iDuration2InMin = 0;

    // unsigned int percentToSpeedRatio = 1200;
    // unsigned int microStep = 128;
    // unsigned int rmsCurrent = 2000;
    // unsigned int acceleration = 2000;
};

struct SpeedViaButton_t 
{
    uint16_t iSpeedLow = 0;
    uint16_t iSpeedMed = 1;
    uint16_t iSpeedHigh = 2;
};

struct DateTime_t {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    // uint8_t second;
};

enum THERMAL_STATUS {
    THERMAL_STATUS_UNDEFINED = 0,
    THERMAL_STATUS_ON,
    THERMAL_STATUS_OFF
};

enum E_DEBUG_LEVEL {
    E_DEBUG_LEVEL_OFF = 0,
    E_DEBUG_LEVEL_DEBUG
};

struct StepperConfiguration_t
{
    unsigned int percentToSpeedRatio = 0;
    unsigned int microStep = 0;
    unsigned int rmsCurrent = 0;
    unsigned int acceleration = 0;
    uint8_t thermalEnable = 0;
    uint8_t blowerVolt = 0;
    uint8_t stallTime = 0;
    uint8_t mpuIdleTime = 0;
    uint8_t gThreshold = (int)(0 * 100);
    uint8_t overHeatTime = 0;
    SpeedViaButton_t speedViaButton;
    uint8_t eDebugLevel = 0;
    uint8_t stallGuard4Threshold = 0;
};


struct ModuleConnectStatus_t
{
    bool hasMPU = false;
    bool hasSDCard = false;
};

enum STEPPER_PROTECTION_CODE {
    STEPPER_PROTECTION_CODE_TOO_HOT = 0,
    STEPPER_PROTECTION_CODE_STALLED
};

enum E_DRIVER_REPORT_STATUS {
    E_DRIVER_REPORT_STATUS_RESTARTED = 0,
    E_DRIVER_REPORT_STATUS_STARTED,
    E_DRIVER_REPORT_STATUS_STOPPED,
    E_DRIVER_REPORT_STATUS_MAX
};

typedef void (*callback_t)();
typedef void (*callback_protection_t)(const E_DRIVER_REPORT_STATUS code);
typedef void (*callback_t_config)(StepperConfiguration_t &config);
typedef void (*callback_t_param)(const uint32_t duration);
typedef void (*callback_t_cmd)(String cmd, const ControlDataMotor_t value);

#endif