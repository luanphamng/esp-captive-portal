#include <Arduino.h>
#include "DebugConfig.h"

E_DEBUG_LEVEL m_debugLevel = E_DEBUG_LEVEL_DEBUG;

void setDebugLevel(uint8_t _eDebugLevel) {
    m_debugLevel = (E_DEBUG_LEVEL)_eDebugLevel;
}

void SerialPrintDebug(bool isNewLine, String str) {
#ifdef DEBUG_MODE
    if (m_debugLevel > 0) {
        if (isNewLine)
            Serial.println(str);
        else
            Serial.print(str);
    }
#endif
}
