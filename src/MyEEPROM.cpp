#include "MyEEPROM.h"
#include "DebugConfig.h"
#include "DataMsgDefine.h"
#include "StepperMotorConfig.h"

namespace espEeprom
{
void begin()
{
    EEPROM.begin(EEPROM_SIZE);
}
// ===== Adaptive EEPROM  =====
void myEEPROMWrite(int const addr, int16_t data)
{
    if (data >= 0)
    {
        EEPROM.write(addr, 0);
        EEPROM.write(addr + 4, (uint8_t)(data & 0xFF));
        EEPROM.write(addr + 8, (uint8_t)((data >> 8) & 0xFF));
    }
    else
    {
        int uData = abs(data);
        EEPROM.write(addr, 1);
        EEPROM.write(addr + 4, (uint8_t)(uData & 0xFF));
        EEPROM.write(addr + 8, (uint8_t)((uData >> 8) & 0xFF));
    }
    // EEPROM.commit();
}

void myEEPROMWriteByte(int const addr, int8_t data) {
    EEPROM.write(addr, data);
}

int16_t myEEPROMRead(int const addr)
{
    int sign = EEPROM.read(addr);
    if (sign != 0)
    {
        sign = -1;
    }
    else
    {
        sign = 1;
    }
    uint8_t ls = EEPROM.read(addr + 4);
    uint8_t ms = EEPROM.read(addr + 8);
    return sign * (ls + ms * 256);
}

uint8_t myEEPROMReadByte(int const addr)
{
    return (EEPROM.read(addr));
}

void saveMotorSettingToEEPROM(const StepperConfiguration_t &tMotorSettingdata)
{
    myEEPROMWrite(EEPROM_ADDR_SETTING_PERCENT2SPEED, tMotorSettingdata.percentToSpeedRatio);
    myEEPROMWrite(EEPROM_ADDR_SETTING_MICRO_STEP, tMotorSettingdata.microStep);
    myEEPROMWrite(EEPROM_ADDR_SETTING_RMS_CURRENT, tMotorSettingdata.rmsCurrent);
    myEEPROMWrite(EEPROM_ADDR_SETTING_ACC, tMotorSettingdata.acceleration);
    
    myEEPROMWriteByte(EEPROM_ADDR_SETTING_BYTE_THERMAL_EN, tMotorSettingdata.thermalEnable + 1);
    myEEPROMWriteByte(EEPROM_ADDR_SETTING_BYTE_BLOWER_VOLT, tMotorSettingdata.blowerVolt + 1);
    myEEPROMWriteByte(EEPROM_ADDR_SETTING_BYTE_STALL_TIME, tMotorSettingdata.stallTime);
    myEEPROMWriteByte(EEPROM_ADDR_SETTING_BYTE_G_THRESHOLD, tMotorSettingdata.gThreshold + 1);
    myEEPROMWriteByte(EEPROM_ADDR_SETTING_BYTE_MPU_IDLE, tMotorSettingdata.mpuIdleTime);
    myEEPROMWriteByte(EEPROM_ADDR_SETTING_BYTE_OVERHEAT_TIME, tMotorSettingdata.overHeatTime);

    myEEPROMWriteByte(EEPROM_ADDR_SETTING_BYTE_BUTTON_SPEED_LOW, tMotorSettingdata.speedViaButton.iSpeedLow);
    myEEPROMWriteByte(EEPROM_ADDR_SETTING_BYTE_BUTTON_SPEED_MED, tMotorSettingdata.speedViaButton.iSpeedMed);
    myEEPROMWriteByte(EEPROM_ADDR_SETTING_BYTE_BUTTON_SPEED_HIGH, tMotorSettingdata.speedViaButton.iSpeedHigh);
    myEEPROMWriteByte(EEPROM_ADDR_SETTING_BYTE_DEBUG_LEVEL, tMotorSettingdata.eDebugLevel + 1);
    myEEPROMWriteByte(EEPROM_ADDR_SETTING_STALLGUARD4, tMotorSettingdata.stallGuard4Threshold + 1);
    EEPROM.commit();

    SerialPrintDebug(true, "Save EEPROM - percent: " + String(tMotorSettingdata.percentToSpeedRatio) +
                                        ", step: " + String(tMotorSettingdata.microStep) +
                                        ", rms: " + String(tMotorSettingdata.rmsCurrent) +
                                        ", acc(s): " + String(tMotorSettingdata.acceleration) +
                                        ", thermal: " + String(tMotorSettingdata.thermalEnable) +
                                        ", blowerVolt: " + String(tMotorSettingdata.blowerVolt) +
                                        ", stallTime: " + String(tMotorSettingdata.stallTime) +
                                        ", gThreshold: " + String(tMotorSettingdata.gThreshold) +
                                        ", mpuIdleTime: " + String(tMotorSettingdata.mpuIdleTime) +
                                        ", overHeatTime: " + String(tMotorSettingdata.overHeatTime) +
                                        ", debug: " + String(tMotorSettingdata.eDebugLevel) +
                                        ", SG4: " + String(tMotorSettingdata.stallGuard4Threshold)
                                        );

    SerialPrintDebug(true, "----------- - , Button speed Low: " + String(tMotorSettingdata.speedViaButton.iSpeedLow) +
                                                     ", Med: " + String(tMotorSettingdata.speedViaButton.iSpeedMed) +
                                                     ", High: " + String(tMotorSettingdata.speedViaButton.iSpeedHigh)
                                                     );
    SerialPrintDebug(true, "Set debug level to: " + String(tMotorSettingdata.eDebugLevel));
    setDebugLevel(tMotorSettingdata.eDebugLevel);
}

void loadMotorSettingFromEEPROM(StepperConfiguration_t &tMotorSettingdata)
{
    tMotorSettingdata.percentToSpeedRatio = myEEPROMRead(EEPROM_ADDR_SETTING_PERCENT2SPEED);
    if (tMotorSettingdata.percentToSpeedRatio < LOWER_LIMIT_PERCENT_TO_SPEED_RATIO || tMotorSettingdata.percentToSpeedRatio  > UPPER_LIMIT_PERCENT_TO_SPEED_RATIO)
        tMotorSettingdata.percentToSpeedRatio = DEFAULT_PERCENT_TO_SPEED_RATIO;

    tMotorSettingdata.microStep = myEEPROMRead(EEPROM_ADDR_SETTING_MICRO_STEP);
    if (tMotorSettingdata.microStep < LOWER_LIMIT_MICROSTEP || tMotorSettingdata.microStep > UPPER_LIMIT_MICROSTEP)
        tMotorSettingdata.microStep = DEFAULT_MICROSTEP;

    tMotorSettingdata.rmsCurrent = myEEPROMRead(EEPROM_ADDR_SETTING_RMS_CURRENT);
    if (tMotorSettingdata.rmsCurrent < LOWER_LIMIT_RMS_CURRENT || tMotorSettingdata.rmsCurrent > UPPER_LIMIT_RMS_CURRENT)
        tMotorSettingdata.rmsCurrent = DEFAULT_RMS_CURRENT;

    tMotorSettingdata.acceleration = myEEPROMRead(EEPROM_ADDR_SETTING_ACC);
    if (tMotorSettingdata.acceleration < LOWER_LIMIT_ACCELERATION_IN_SEC || tMotorSettingdata.acceleration > UPPER_LIMIT_ACCELERATION_IN_SEC)
        tMotorSettingdata.acceleration = DEFAULT_ACCELERATION_IN_SEC;

    tMotorSettingdata.thermalEnable = myEEPROMReadByte(EEPROM_ADDR_SETTING_BYTE_THERMAL_EN);
    if (tMotorSettingdata.thermalEnable == 0 || tMotorSettingdata.thermalEnable > 2)
        tMotorSettingdata.thermalEnable = DEFAULT_THERMAL_ENABLE;
     else
        tMotorSettingdata.thermalEnable -= 1;

    tMotorSettingdata.blowerVolt = myEEPROMReadByte(EEPROM_ADDR_SETTING_BYTE_BLOWER_VOLT);
    // Blower actual range: 0 ... 5
    // Blower when save to EEPROM 1 .. 6, because need to detect default value when EEPROM not saved (EEPROM Read result  = 0.)
    if (tMotorSettingdata.blowerVolt < (LOWER_LIMIT_BLOWER_VOLT + 1) || tMotorSettingdata.blowerVolt > (UPPER_LIMIT_BUPPER_VOLT + 1)) {
        tMotorSettingdata.blowerVolt = DEFAULT_BLOWER_VOLT;
    } else {
        tMotorSettingdata.blowerVolt -= 1;
    }

    tMotorSettingdata.stallTime = myEEPROMReadByte(EEPROM_ADDR_SETTING_BYTE_STALL_TIME);
    if (tMotorSettingdata.stallTime < LOWER_LIMIT_STALL_TIME || tMotorSettingdata.stallTime > UPPER_LIMIT_STALL_TIME)
        tMotorSettingdata.stallTime = DEFAULT_STALL_TIME;

    tMotorSettingdata.gThreshold = myEEPROMReadByte(EEPROM_ADDR_SETTING_BYTE_G_THRESHOLD);
    if (tMotorSettingdata.gThreshold < (LOWER_LIMIT_G_THRESHOLD + 1) || tMotorSettingdata.gThreshold > (UPPER_LIMIT_G_THRESHOLD + 1))
        tMotorSettingdata.gThreshold = (int)(DEFAULT_STALL_GUARD_VIA_ACCEL_THRESHOLD * 100);
    else
        tMotorSettingdata.gThreshold -= 1;
    
    tMotorSettingdata.mpuIdleTime = myEEPROMReadByte(EEPROM_ADDR_SETTING_BYTE_MPU_IDLE);
    if (tMotorSettingdata.mpuIdleTime < LOWER_LIMIT_MPU_IDLE_TIME || tMotorSettingdata.mpuIdleTime > UPPER_LIMIT_MPU_IDLE_TIME)
        tMotorSettingdata.mpuIdleTime = DEFAULT_MPU_IDLE_TIME;

    tMotorSettingdata.overHeatTime = myEEPROMReadByte(EEPROM_ADDR_SETTING_BYTE_OVERHEAT_TIME);
    if (tMotorSettingdata.overHeatTime < LOWER_LIMIT_OVERHEAT_TIME || tMotorSettingdata.overHeatTime > UPPER_LIMIT_OVERHEAT_TIME)
        tMotorSettingdata.overHeatTime = DEFAULT_OVERHEAT_TIME;

    tMotorSettingdata.speedViaButton.iSpeedLow = myEEPROMReadByte(EEPROM_ADDR_SETTING_BYTE_BUTTON_SPEED_LOW);
    if (tMotorSettingdata.speedViaButton.iSpeedLow < LOWER_LIMIT_SPEED || tMotorSettingdata.speedViaButton.iSpeedLow > UPPER_LIMIT_SPEED)
        tMotorSettingdata.speedViaButton.iSpeedLow = DEFAULT_STEPPER_SPEED_LOW;

    tMotorSettingdata.speedViaButton.iSpeedMed = myEEPROMReadByte(EEPROM_ADDR_SETTING_BYTE_BUTTON_SPEED_MED);
    if (tMotorSettingdata.speedViaButton.iSpeedMed < LOWER_LIMIT_SPEED || tMotorSettingdata.speedViaButton.iSpeedMed > UPPER_LIMIT_SPEED)
        tMotorSettingdata.speedViaButton.iSpeedMed = DEFAULT_STEPPER_SPEED_MEDIUM;

    tMotorSettingdata.speedViaButton.iSpeedHigh = myEEPROMReadByte(EEPROM_ADDR_SETTING_BYTE_BUTTON_SPEED_HIGH);
    if (tMotorSettingdata.speedViaButton.iSpeedHigh < LOWER_LIMIT_SPEED || tMotorSettingdata.speedViaButton.iSpeedHigh > UPPER_LIMIT_SPEED)
        tMotorSettingdata.speedViaButton.iSpeedHigh = DEFAULT_STEPPER_SPEED_HIGH;
    
    tMotorSettingdata.eDebugLevel = myEEPROMReadByte(EEPROM_ADDR_SETTING_BYTE_DEBUG_LEVEL);
    if ((tMotorSettingdata.eDebugLevel <= 0) || tMotorSettingdata.eDebugLevel > (E_DEBUG_LEVEL_DEBUG + 1))
        tMotorSettingdata.eDebugLevel = E_DEBUG_LEVEL_OFF;
    else
        tMotorSettingdata.eDebugLevel -= 1;

    tMotorSettingdata.stallGuard4Threshold = myEEPROMReadByte(EEPROM_ADDR_SETTING_STALLGUARD4);
    if (tMotorSettingdata.stallGuard4Threshold <= LOWER_STALLGUARD4_THRESHOLD || tMotorSettingdata.stallGuard4Threshold > (UPPER_STALLGUARD4_THRESHOLD + 1)) {
        tMotorSettingdata.stallGuard4Threshold = DEFAULT_STALLGUARD4_THRESHOLD;
    } else {
        tMotorSettingdata.stallGuard4Threshold-= 1;
    }
    SerialPrintDebug(true, "Load EEPROM - percent: " + String(tMotorSettingdata.percentToSpeedRatio) +
                                        ", step: " + String(tMotorSettingdata.microStep) +
                                        ", rms: " + String(tMotorSettingdata.rmsCurrent) +
                                        ", acc(s): " + String(tMotorSettingdata.acceleration) +
                                        ", thermal: " + String(tMotorSettingdata.thermalEnable) +
                                        ", blowerVolt: " + String(tMotorSettingdata.blowerVolt) +
                                        ", stallTime: " + String(tMotorSettingdata.stallTime) +
                                        ", gThreshold: " + String(tMotorSettingdata.gThreshold) +
                                        ", mpuIdleTime: " + String(tMotorSettingdata.mpuIdleTime) +
                                        ", overHeatTime: " + String(tMotorSettingdata.overHeatTime) +
                                        ", debugLevel: " + String(tMotorSettingdata.eDebugLevel) +
                                        ", SG4: " + String(tMotorSettingdata.stallGuard4Threshold)
                                        );

    SerialPrintDebug(true, "----------- - , Button speed Low: " + String(tMotorSettingdata.speedViaButton.iSpeedLow) +
                                                     ", Med: " + String(tMotorSettingdata.speedViaButton.iSpeedMed) +
                                                     ", High: " + String(tMotorSettingdata.speedViaButton.iSpeedHigh)
                                                     );
    SerialPrintDebug(true, "Set debug level to: " + String(tMotorSettingdata.eDebugLevel));
    setDebugLevel(tMotorSettingdata.eDebugLevel);
}

void saveUserControlDataToEEPROM(const ControlDataMotor_t &tControlData)
{
    myEEPROMWriteByte(EEPROM_ADDR_CONTROL_BYTE_SPEED, tControlData.iSpeed);
    myEEPROMWrite(EEPROM_ADDR_CONTROL_DURATION, tControlData.iDurationInMin);
    myEEPROMWriteByte(EEPROM_ADDR_CONTROL_BYTE_SENS, tControlData.iSensitivity + 1);
    myEEPROMWriteByte(EEPROM_ADDR_CONTROL_BYTE_AUTOSPEED, tControlData.iAutoSpeed + 1);
    myEEPROMWrite(EEPROM_ADDR_CONTROL_DURATION2, tControlData.iDuration2InMin);
    EEPROM.commit();

    SerialPrintDebug(true, "Save EEPROM - speed: " + String(tControlData.iSpeed) + 
                                        ", duration: " + String(tControlData.iDurationInMin) +
                                        ", sens: " + String(tControlData.iSensitivity) + 
                                        ", autospeed: " + String(tControlData.iAutoSpeed) + 
                                        ", duration2: " + String(tControlData.iDuration2InMin));
}

void loadUserControlDataToEEPROM(ControlDataMotor_t &controlData)
{

    controlData.iSpeed = myEEPROMReadByte(EEPROM_ADDR_CONTROL_BYTE_SPEED);
    if (controlData.iSpeed < LOWER_LIMIT_SPEED || controlData.iSpeed > UPPER_LIMIT_SPEED)
        controlData.iSpeed = DEFAULT_SPEED;

    controlData.iDurationInMin = myEEPROMRead(EEPROM_ADDR_CONTROL_DURATION);
    if (controlData.iDurationInMin < LOWER_LIMIT_DURATION || controlData.iDurationInMin > UPPER_LIMIT_DURATION)
        controlData.iDurationInMin = DEFAULT_DURATION;

    controlData.iSensitivity = myEEPROMReadByte(EEPROM_ADDR_CONTROL_BYTE_SENS);
    if (controlData.iSensitivity < (LOWER_LIMIT_SENS + 1)|| controlData.iSensitivity > (UPPER_LIMIT_SENS + 1)) {
        controlData.iSensitivity = DEFAULT_SENS;
    } else {
        controlData.iSensitivity -= 1;
    }

    controlData.iDuration2InMin = myEEPROMRead(EEPROM_ADDR_CONTROL_DURATION2);
    if (controlData.iDuration2InMin == 0 || controlData.iDuration2InMin > 120)
        controlData.iDuration2InMin = DEFAULT_AUTOTIME;

    controlData.iAutoSpeed = myEEPROMReadByte(EEPROM_ADDR_CONTROL_BYTE_AUTOSPEED);
    if (controlData.iAutoSpeed == 0 || controlData.iAutoSpeed > (100 + 1)) {
        controlData.iAutoSpeed = DEFAULT_AUTOSPEED;
    }
    else {
        controlData.iAutoSpeed -= 1;
    }

    SerialPrintDebug(true, "Load EEPROM - speed: " + String(controlData.iSpeed) + 
                                        ", duration: " + String(controlData.iDurationInMin) +
                                        ", sens: " + String(controlData.iSensitivity) + 
                                        ", autospeed: " + String(controlData.iAutoSpeed) + 
                                        ", duration2: " + String(controlData.iDuration2InMin));
}

    void saveWifiSettingToEEProm(const uint8_t wifiState) {
        SerialPrintDebug(true, "Save EEPROM - Wifi: " + String(wifiState));
        myEEPROMWriteByte(EEPROM_ADDR_BYTE_WIFI_STATE, wifiState + 1);
        EEPROM.commit();
    }

    void loadWifiSettingFromEEProm(uint8_t &wifiState) {
        wifiState = myEEPROMReadByte(EEPROM_ADDR_BYTE_WIFI_STATE);
        if (wifiState == 0 || wifiState > 2)
            wifiState = DEFAUT_WIFI_ENABLE;
        else
            wifiState-=1;

        SerialPrintDebug(true, "Load EEPROM - Wifi: " + String(wifiState));
    }
} // end namespace