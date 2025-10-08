#include <WiFi.h>
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "esp_wps.h"
#include "esp_wifi.h"
#include "DebugConfig.h"

// char MyWiFi_ssid[30];
const char* Default_SSID = "EspOpen.com";
const char* MyWiFi_password =  "a@A12345";

IPAddress apIP(192,168,4,1);

static void initWifi() {
    SerialPrintDebug(true, "1. Disconnect Wifi");
    uint16_t chip = (uint16_t)(ESP.getEfuseMac() >> 32);
    // WiFi.mode(WIFI_AP);

    String apName = String(Default_SSID);
    SerialPrintDebug(true, "    1.1 Wifi mode: Done");
    delay(500);
    // Create wifi SSID base on MAC Address
    SerialPrintDebug(true, "    1.1 concat chipID to SSID");
    apName.concat(chip);
    // strcat(MyWiFi_ssid, Default_SSID);
    // strcat(MyWiFi_ssid, WiFi.macAddress().c_str());
    SerialPrintDebug(true, "    1.1 SSID: " + apName);
    WiFi.softAP(apName.c_str(), MyWiFi_password);
    SerialPrintDebug(true, "    1.2 SoftAP: Done");
    delay(500);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0));
    SerialPrintDebug(true, "    1.3 SoftAP Config: Done");
    delay(500);
}

static void display_connected_devices()
{
#if DEBUG_MODE
    wifi_sta_list_t wifi_sta_list;
    tcpip_adapter_sta_list_t adapter_sta_list;
    esp_wifi_ap_get_sta_list(&wifi_sta_list);
    tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);

    if (adapter_sta_list.num > 0) {
        SerialPrintDebug(true, "-----------");
    } else {
        SerialPrintDebug(true, "No devices connected");
        return;
    }

    for (uint8_t i = 0; i < adapter_sta_list.num; i++)
    {
        tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
        SerialPrintDebug(false, "[+] Device " + String(i) + " | MAC : ");
        // SerialPrintDebug(false, String(station.mac[0]) + ":" + String(station.mac[1]) + ":" + String(station.mac[2]) +  ":" + 
        //                         String(station.mac[3]) + ":" + String(station.mac[4]) + String(station.mac[5]));
        //SerialPrintDebug(true, (String) " | IP " + ip4addr_ntoa(&(station.ip)));
    }
#endif
}

static void stopWifi() {
    SerialPrintDebug(true, "Stop wifi");
    WiFi.mode(WIFI_OFF);
}
