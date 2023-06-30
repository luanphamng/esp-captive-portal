#ifndef DEBUG_CONFIG_H
#define DEBUG_CONFIG_H

#include "DataMsgDefine.h"
#include <Arduino.h>

void setDebugLevel(uint8_t _eDebugLevel);

void SerialPrintDebug(bool isNewLine, String str);

#endif
