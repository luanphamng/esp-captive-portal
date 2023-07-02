#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

/* =========== OTA ============ */
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <Update.h>

#include "WebServer.h"
#include "WiFiConfig.h"
#include "MyEEPROM.h"
#include "StepperMotorConfig.h"
#include "DataMsgDefine.h"

StepperConfiguration_t g_config;
ControlDataMotor_t g_controlData;

void saveConfigToSpiffs(StepperConfiguration_t &config)
{
    SerialPrintDebug(true, "Driver setting handler");
    g_config = config;
    espEeprom::saveMotorSettingToEEPROM(g_config);
}

//------------------ Control page action handler ------------------------
void commandHandler(String cmd, ControlDataMotor_t controlData)
{
    espEeprom::saveUserControlDataToEEPROM(controlData);
    SerialPrintDebug(true, "Received command: " + cmd);
}

void setup() {
    delay(2000); // Stability
    // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
#if DEBUG_MODE
    Serial.begin(115200);
    // while (!Serial);
#endif
    espEeprom::begin();

    initWifi();

    startWebserver(&g_controlData, saveConfigToSpiffs, commandHandler);
}


void loop() {
    
}