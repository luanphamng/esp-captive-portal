#ifndef MY_EEPROM_H
#define MY_EEPROM_H

#include <EEPROM.h>
#include "DataMsgDefine.h"

#define EEPROM_SIZE 278 // bytes

/* Address for setting page */

#define EEPROM_ADDR_START_ADDRESS               0
#define EEPROM_ADDR_SETTING_PERCENT2SPEED       EEPROM_ADDR_START_ADDRESS + 4               // 2 bytes
#define EEPROM_ADDR_SETTING_MICRO_STEP          EEPROM_ADDR_SETTING_PERCENT2SPEED + 16      // 2 bytes
#define EEPROM_ADDR_SETTING_RMS_CURRENT         EEPROM_ADDR_SETTING_MICRO_STEP + 16         // 2 bytes
#define EEPROM_ADDR_SETTING_ACC                 EEPROM_ADDR_SETTING_RMS_CURRENT + 16        // 2 bytes

#define EEPROM_ADDR_SETTING_BYTE_BUTTON_SPEED_LOW    EEPROM_ADDR_SETTING_ACC + 16                   // 1 bytes
#define EEPROM_ADDR_SETTING_BYTE_BUTTON_SPEED_MED    EEPROM_ADDR_SETTING_BYTE_BUTTON_SPEED_LOW + 4       // 1 bytes
#define EEPROM_ADDR_SETTING_BYTE_BUTTON_SPEED_HIGH   EEPROM_ADDR_SETTING_BYTE_BUTTON_SPEED_MED + 4       // 1 bytes

#define EEPROM_ADDR_SETTING_BYTE_THERMAL_EN     EEPROM_ADDR_SETTING_BYTE_BUTTON_SPEED_HIGH + 4  // 1 byte
#define EEPROM_ADDR_SETTING_BYTE_BLOWER_VOLT    EEPROM_ADDR_SETTING_BYTE_THERMAL_EN + 4     // 1 byte
#define EEPROM_ADDR_SETTING_BYTE_STALL_TIME     EEPROM_ADDR_SETTING_BYTE_BLOWER_VOLT + 4    // 1 byte
#define EEPROM_ADDR_SETTING_BYTE_G_THRESHOLD    EEPROM_ADDR_SETTING_BYTE_STALL_TIME + 4     // 1 byte
#define EEPROM_ADDR_SETTING_BYTE_MPU_IDLE       EEPROM_ADDR_SETTING_BYTE_G_THRESHOLD + 4    // 1 byte
#define EEPROM_ADDR_SETTING_BYTE_OVERHEAT_TIME  EEPROM_ADDR_SETTING_BYTE_MPU_IDLE + 4       // 1 byte
#define EEPROM_ADDR_SETTING_BYTE_DEBUG_LEVEL    EEPROM_ADDR_SETTING_BYTE_OVERHEAT_TIME + 4  // 1 byte
#define EEPROM_ADDR_SETTING_STALLGUARD4         EEPROM_ADDR_SETTING_BYTE_DEBUG_LEVEL + 4    // 1 byte

// Address for control page
#define EEPROM_ADDR_CONTROL_BYTE_SPEED          EEPROM_ADDR_SETTING_STALLGUARD4 + 4         // 1 byte
#define EEPROM_ADDR_CONTROL_BYTE_SENS           EEPROM_ADDR_CONTROL_BYTE_SPEED + 4          // 1 byte
#define EEPROM_ADDR_CONTROL_BYTE_AUTOSPEED      EEPROM_ADDR_CONTROL_BYTE_SENS + 4           // 2 bytes

#define EEPROM_ADDR_CONTROL_DURATION            EEPROM_ADDR_CONTROL_BYTE_AUTOSPEED + 4      // 2 bytes
#define EEPROM_ADDR_CONTROL_DURATION2           EEPROM_ADDR_CONTROL_DURATION + 16           // 2 bytes

// Address for wifi setting
#define EEPROM_ADDR_BYTE_WIFI_STATE                  EEPROM_ADDR_CONTROL_DURATION2 + 16           // 1 bytes
namespace espEeprom
{
    void begin();
    // ===== Adaptive EEPROM  =====
    void myEEPROMWriteByte(int const addr, int8_t data);
    void myEEPROMWrite(int const addr, int16_t data);

    uint8_t myEEPROMReadByte(int const addr);
    int16_t myEEPROMRead(int const addr);

    void saveMotorSettingToEEPROM(const StepperConfiguration_t &tMotorSettingdata);
    void saveUserControlDataToEEPROM(const ControlDataMotor_t &tControlData);    
    void loadMotorSettingFromEEPROM(StepperConfiguration_t &tMotorSettingdata);
    void loadUserControlDataToEEPROM(ControlDataMotor_t &controlData);

    void saveWifiSettingToEEProm(const uint8_t wifiState);
    void loadWifiSettingFromEEProm(uint8_t &wifiState);
}

#endif