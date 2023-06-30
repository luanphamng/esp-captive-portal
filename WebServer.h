#ifndef _WEB_SERVER_H
#define _WEB_SERVER_H


#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "DataMsgDefine.h"


void startWebserver(ControlDataMotor_t *config, callback_t_config saveConfigCallback, callback_t_cmd commandCallback);
void registerWebInterfaceUrls(AsyncWebServer *httpServer);
void registerRestApiUrls(AsyncWebServer *httpServer);
void handleTasks();
void startSPIFFS();
void loadSavedData();
void notifyStepperRunningStatusChange_ToWebserver(const bool running);
void notifyDurationChanged_ToWebserver(const uint16_t& duration);
void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
#endif