#include "WebServer.h"
#include "DebugConfig.h"
#include "MyEEPROM.h"
#include <WiFi.h>

const char *webUiHtml = "/index.html";
const char *webUiCaptivePortal = "/captive.html";

using namespace espEeprom;

ControlDataMotor_t controlData = {};
ControlDataMotor_t prevControlData = controlData;
StepperConfiguration_t configData;

AsyncWebServer webserver(80);

DNSServer dnsServer;
class CaptiveRequestHandler : public AsyncWebHandler
{
public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, webUiCaptivePortal, "text/html");
        request->send(response);
    }
};

callback_t_config _saveConfigCallback;
callback_t_cmd _commandCallback;

void buildStatusResponse(String *reponseString, int rc, const char *message)
{
    reponseString->concat("{\"status\" : ");
    reponseString->concat(rc);
    reponseString->concat(", \"message\": \"");
    reponseString->concat(message);
    reponseString->concat("\"}");
}

void sendResponse(AsyncWebServerRequest *request, int rc, const char *message)
{
    String responseBody;
    buildStatusResponse(&responseBody, rc, message);
    request->send(rc, "application/json", responseBody);
}
void loadSavedData() {
    espEeprom::loadMotorSettingFromEEPROM(configData);
    espEeprom::loadUserControlDataToEEPROM(controlData);
}
void startWebserver(ControlDataMotor_t *config, callback_t_config saveConfigCallback, callback_t_cmd commandCallback)
{
    // loadSavedData();
    // _saveConfigCallback = saveConfigCallback;
    // _commandCallback = commandCallback;
    SerialPrintDebug(true, "Starting Webserver on port 80");
    registerRestApiUrls(&webserver);
    registerWebInterfaceUrls(&webserver);
    SerialPrintDebug(true, "Service Endpoints registered");
    dnsServer.start(53, "*", WiFi.softAPIP());  // Port 53: DNS
    webserver.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
    webserver.begin();
}

void handleTasks()
{
    dnsServer.processNextRequest();
}

void registerRestApiUrls(AsyncWebServer *server)
{
    server->on("/control1", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    if(request->hasParam("speed", true)){
        controlData.isStart = 1;
        const AsyncWebParameter* pSpeed = request->getParam("speed", true);
        controlData.iSpeed = pSpeed->value().toInt();
        const AsyncWebParameter* pSensitivity = request->getParam("sensitivity", true);
        controlData.iSensitivity = pSensitivity->value().toInt();
        const AsyncWebParameter* pDurationInSec = request->getParam("durationInSec", true);
        controlData.iDurationInMin = pDurationInSec->value().toInt();
        _commandCallback("speed", controlData);
     }
    request->send(200); });

    server->on("/control2", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    if(request->hasParam("speed", true)){
        controlData.isStart = 1;
        const AsyncWebParameter* pSpeed = request->getParam("speed", true);
        controlData.iSpeed = pSpeed->value().toInt();

        const AsyncWebParameter* pSensitivity = request->getParam("sensitivity", true);
        controlData.iSensitivity = pSensitivity->value().toInt();

        const AsyncWebParameter* pDurationInSec = request->getParam("durationInSec", true);
        controlData.iDurationInMin = pDurationInSec->value().toInt();

        const AsyncWebParameter* pAutoTimeInSec = request->getParam("autoTimeInSec", true);
        controlData.iDuration2InMin = pAutoTimeInSec->value().toInt();

        const AsyncWebParameter* pAutoSpeed = request->getParam("autoSpeed", true);
        controlData.iAutoSpeed = pAutoSpeed->value().toInt();

        SerialPrintDebug(true, "HTTP POST /start: Received - speed: " + String(controlData.iSpeed) +
                                                        ", sens: " + String(controlData.iSensitivity)+ 
                                                        ", duration: " + String(controlData.iDurationInMin) + 
                                                        ", autospd: " + String(controlData.iAutoSpeed) + 
                                                        ", duration2: " + String(controlData.iDuration2InMin)
                                                        );
        _commandCallback("start", controlData);

     }
    request->send(200); });

    server->on("/downloadToDriver", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    if(request->hasParam("resetDefault", true)){
        StepperConfiguration_t _defaultConfig;
        configData = _defaultConfig;
        SerialPrintDebug(true, "HTTP POST /downloadToDriver - Reset setting to default");
    } else if (request->hasParam("percentToSpeedRatio", true)) {
        const AsyncWebParameter* pPercent2Speed = request->getParam("percentToSpeedRatio", true);
        configData.percentToSpeedRatio = pPercent2Speed->value().toInt();

        const AsyncWebParameter* pMicroStep = request->getParam("microStep", true);
        configData.microStep = pMicroStep->value().toInt();
        
        const AsyncWebParameter* pRmsCurrent = request->getParam("rmsCurrent", true);
        configData.rmsCurrent = pRmsCurrent->value().toInt();

        const AsyncWebParameter* pAcceleration = request->getParam("acceleration", true);
        configData.acceleration = pAcceleration->value().toInt();

        const AsyncWebParameter* pBlowerVolt = request->getParam("blowerVolt", true);
        configData.blowerVolt = pBlowerVolt->value().toInt();

        const AsyncWebParameter* pThermalEnable = request->getParam("thermalEnable", true);
        configData.thermalEnable = pThermalEnable->value().toInt();

        const AsyncWebParameter* pStallTime = request->getParam("stallTime", true);
        configData.stallTime = pStallTime->value().toInt();

        const AsyncWebParameter* pGThreshold = request->getParam("gThreshold", true);
        configData.gThreshold = pGThreshold->value().toInt();

        const AsyncWebParameter* pMpuIdleTime = request->getParam("mpuIdleTime", true);
        configData.mpuIdleTime = pMpuIdleTime->value().toInt();

        const AsyncWebParameter* pOverHeatTime = request->getParam("overHeatTime", true);
        configData.overHeatTime = pOverHeatTime->value().toInt();

        const AsyncWebParameter* pDebugLevel = request->getParam("debugLevel", true);
        configData.eDebugLevel = pDebugLevel->value().toInt();

        const AsyncWebParameter* pSG4 = request->getParam("stallGuard4Threshold", true);
        configData.stallGuard4Threshold = pSG4->value().toInt();
        // Button via speed
        const AsyncWebParameter* pButtonSpeedLow = request->getParam("buttonViaSpeed_low", true);
        configData.speedViaButton.iSpeedLow = pButtonSpeedLow->value().toInt();

        const AsyncWebParameter* pButtonSpeedmed = request->getParam("buttonViaSpeed_med", true);
        configData.speedViaButton.iSpeedMed = pButtonSpeedmed->value().toInt();

        const AsyncWebParameter* pButtonSpeedHigh = request->getParam("buttonViaSpeed_high", true);
        configData.speedViaButton.iSpeedHigh = pButtonSpeedHigh->value().toInt();

    } else {

    }
 
    _saveConfigCallback(configData);
    
    SerialPrintDebug(true, "HTTP POST /downloadToDriver - percent: " + String(configData.percentToSpeedRatio) + ", microStep: " + String(configData.microStep)
    + ", RMS: " + String(configData.rmsCurrent) + ", acc(s): " + String(configData.acceleration) + ", thermalEN: " + String(configData.thermalEnable));
    
    request->send(200); });

    server->on("/stop", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    controlData.isStart = 0;
    _commandCallback("stop", controlData);

    request->send(200); });
    
    server->on("/off", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    controlData.isStart = 0;
    _commandCallback("off", controlData);

    request->send(200); });

    server->on("/get-current-info", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    SerialPrintDebug(true, "[HTTP-REQ] get-current-info, [ESP-RESPONSE] Running: " + String(controlData.isStart) + ", duration: " + String(controlData.iDurationInMin));
      request->send(200, "text/plain", "{ \"Speed\": "+String(controlData.iSpeed)
      +", \"Sensitivity\": "+String(controlData.iSensitivity)
      +", \"DurationInSec\": "+String(controlData.iDurationInMin)
      +", \"AutoSpeed\": "+String(controlData.iAutoSpeed)
      +", \"AutoTimeInSec\": "+String(controlData.iDuration2InMin)
      +", \"IsRunning\": "+String(controlData.isStart)
      +", \"Duration2InSec\": "+String(controlData.iDuration2InMin)
      +"}"); });

    server->on("/get-update-sync", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    SerialPrintDebug(true, "[HTTP-REQ] get-update-sync, [ESP-RESPONSE] Running: " + String(controlData.isStart) + ", duration: " + String(controlData.iDurationInMin));
    if (controlData.isStart != prevControlData.isStart || controlData.iDurationInMin != prevControlData.iDurationInMin) {
        prevControlData = controlData;
        request->send(200, "text/plain", "{ \"DurationInSec\": "+String(controlData.iDurationInMin)
        +", \"IsRunning\": "+String(controlData.isStart)
        +", \"IsChanged\": true"
        +"}");
    } else {
        request->send(200, "text/plain", "{ \"IsChanged\": false}");
    }
    });

    server->on("/get-driver-info", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    SerialPrintDebug(true, "Client REQ - response get-driver-info: " + String(configData.percentToSpeedRatio) + ", " + String(configData.microStep));
      request->send(200, "text/plain", "{ \"PercentToSpeedRatio\": "+String(configData.percentToSpeedRatio)
      +", \"MicroStep\": "+String(configData.microStep)
      +", \"RMSCurrent\": "+String(configData.rmsCurrent)
      +", \"Acceleration\": "+String(configData.acceleration)
      +", \"ThermalEnable\": "+String(configData.thermalEnable)
      +", \"BlowerVolt\": "+String(configData.blowerVolt)
      +", \"StallTime\": "+String(configData.stallTime)
      +", \"GThreshold\": "+String(configData.gThreshold)
      +", \"MPUIdleTime\": "+String(configData.mpuIdleTime)
      +", \"OverHeatTime\": "+String(configData.overHeatTime)
      +", \"DebugLevel\": "+String(configData.eDebugLevel)
      +", \"Button_SpeedLow\": "+String(configData.speedViaButton.iSpeedLow)
      +", \"Button_SpeedMed\": "+String(configData.speedViaButton.iSpeedMed)
      +", \"Button_SpeedHigh\": "+String(configData.speedViaButton.iSpeedHigh)
      +", \"StallGuard4Threshold\": "+String(configData.stallGuard4Threshold)
      +"}"); });

    // get-software-ver
    server->on("/get-software-ver", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    String _fimwareVer = "1.0.8 (2022-10-07)";  // Ex: 01 00 01 = 1.0.1, 20220910 = 2022-09-10
    SerialPrintDebug(true, "Client REQ - response get-software-ver: " + _fimwareVer);
      request->send(200, "text/plain", _fimwareVer ); 
      });

    // get-chart-data
    server->on("/get-chart-data", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        SerialPrintDebug(true, "Client REQ - response get-chart-data");
        if (request->hasParam("date", true)) {
            const AsyncWebParameter* pDate = request->getParam("date", true);
            String _requestDate = pDate->value();
            SerialPrintDebug(true, "Receive request on date: " + _requestDate);
        }
        request->send(200, "text/plain", "123"); 
    });

    server->on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    SerialPrintDebug(true, "Client REQ - response: reboot - OK");
    request->send(200, "text/plain", "OK" );
    ESP.restart();
    });
}

void registerWebInterfaceUrls(AsyncWebServer *server)
{
    SerialPrintDebug(true, "Checking existance of Web UI files in SPIFFS");

    // SerialPrintDebug(true, "Checking existance of " + String(webUiHtml) + ", exist: " + String(SPIFFS.exists(webUiHtml)));
    // SerialPrintDebug(true, "Checking existance of %s: %i\n", webUiJS, SPIFFS.exists(webUiJS));
    // SerialPrintDebug(true, "Checking existance of %s: %i\n", webUiFavicon, SPIFFS.exists(webUiFavicon));
    // SerialPrintDebug(true, "Checking existance of " + String(webUiCaptivePortal) + ", exist: " + String(SPIFFS.exists(webUiCaptivePortal)));

    server->onNotFound([](AsyncWebServerRequest *request)
                       { request->send(404, "text/javascript", "{\"status\":404,\"message\":\"Not Found\"}"); });
    // UI files
    server->serveStatic("/index.html", SPIFFS, webUiHtml).setCacheControl("Cache-Control: public, max-age=604800");

    // serve static file
    server->serveStatic("/", SPIFFS, "/");
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send(SPIFFS, "/index.html"); });

    // serve static file
    server->serveStatic("/", SPIFFS, "/");
    server->on("/notifications.js", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send(SPIFFS, "/notifications.js"); });

    // serve static file
    server->serveStatic("/", SPIFFS, "/");
    server->on("/notifications.css", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send(SPIFFS, "/notifications.css"); });

    server->serveStatic("/", SPIFFS, webUiHtml);
    server->serveStatic("/index.html", SPIFFS, webUiHtml).setCacheControl("Cache-Control: public, max-age=604800");
    // server->serveStatic("/favicon.ico", SPIFFS, webUiFavicon).setCacheControl("Cache-Control: public, max-age=604800");
    // server->serveStatic("/js/app.js", SPIFFS, webUiJS_static);
    // server->serveStatic("/js/app.js.gz", SPIFFS, webUiJS_static);

    // OTA Update handler
    server->on(
        "/update", HTTP_POST, [](AsyncWebServerRequest *request)
        {
      bool espShouldReboot = !Update.hasError();
      AsyncWebServerResponse *response = request->beginResponse(200, "text/html", espShouldReboot ? "<h1><strong>Update DONE</strong></h1><br><a href='/'>Return Home</a>" : "<h1><strong>Update FAILED</strong></h1><br><a href='/updt'>Retry?</a>");
      response->addHeader("Connection", "close");
      request->send(response); },
        [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
        {
            if (!index)
            {
                SerialPrintDebug(true, "Update Start: " + filename);
                if (!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000))
                {
                    Update.printError(Serial);
                }
            }
            if (!Update.hasError())
            {
            if (Update.write(data, len) != len)
            {
                Update.printError(Serial);
            }
            }
            if (final)
            {
            if (Update.end(true))
            {
                SerialPrintDebug(true, "Update Success" /*: %uB\n", index + len*/);
            }
            else
            {
                Update.printError(Serial);
            }
            }
        });
    
    // OTA Update handler for UI only (SPIFFS)
    server->on(
        "/update-spiffs", HTTP_POST, [](AsyncWebServerRequest *request)
        {
      bool espShouldReboot = !Update.hasError();
      AsyncWebServerResponse *response = request->beginResponse(200, "text/html", espShouldReboot ? "<h1><strong>Update UI DONE</strong></h1><br><a href='/'>Return Home</a>" : "<h1><strong>Update UI FAILED</strong></h1><br><a href='/updt'>Retry?</a>");
      response->addHeader("Connection", "close");
      request->send(response); },
        [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
        {
            if (!index) {
                SerialPrintDebug(true, "Update UI Start: " + filename);
                if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_SPIFFS)) {
                    Update.printError(Serial);
                }
            }

            if (!Update.hasError()) {
                if (Update.write(data, len) != len) {
                    Update.printError(Serial);
                }
            }

            if (final) {
                if (Update.end(true)) {
                    SerialPrintDebug(true, "Update UI Success" /*: %uB\n", index + len*/);
                } else {
                    Update.printError(Serial);
                }
            }
        });

    // run handleUpload function when any file is uploaded
    // TODO: Safety check file extension
    // server->on("/update-spiffs", HTTP_POST, [](AsyncWebServerRequest *request) {
    //     SerialPrintDebug(true, "[HTTP][POST]upload-spiffs");
    //     request->send(200);
    //   }, handleFileUpload);

}

void startSPIFFS() {
    // mount ESP32 file system
    if (SPIFFS.begin()) {
        SerialPrintDebug(true, "SPIFFS is mounted successfully");
    } else {
        SerialPrintDebug(true, "An Error has occurred while mounting SPIFFS");
    }
}

void notifyHardwareStatusToWebserver(const ControlDataMotor_t& ctrlData) {
    if (controlData.iDurationInMin != 0)
        controlData.iDurationInMin = ctrlData.iDurationInMin;
    else
        controlData.isStart = ctrlData.isStart;
}

void notifyStepperRunningStatusChange_ToWebserver(const bool running) {
    SerialPrintDebug(true, "notifyStepperRunningStatusChange_ToWebserver: " + String(running));
    controlData.isStart = running;
}
void notifyDurationChanged_ToWebserver(const uint16_t& duration) {
    SerialPrintDebug(true, "notifyDurationChanged_ToWebserver: " + String(duration));
    controlData.iDurationInMin = duration;
}

// handles uploads
void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
  SerialPrintDebug(true, logmessage);

  if (!index) {
    logmessage = "Upload Start: " + String(filename);
    // open the file on first call and store the file handle in the request object
    request->_tempFile = SPIFFS.open("/" + filename, "w");
    SerialPrintDebug(true, logmessage);
  }

  if (len) {
    // stream the incoming chunk to the opened file
    request->_tempFile.write(data, len);
    logmessage = "Writing file: " + String(filename) + " index=" + String(index) + " len=" + String(len);
    SerialPrintDebug(true, logmessage);
  }

  if (final) {
    logmessage = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
    // close the file handle as the upload is now done
    request->_tempFile.close();
    SerialPrintDebug(true, logmessage);
    request->redirect("/");
  }
}