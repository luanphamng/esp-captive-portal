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

void setup() {
    initWifi();
}


void loop() {
    
}