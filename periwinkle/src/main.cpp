#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <PubSubClient.h>
#include "config.esp"

//---------------------------------SYSTEM DECLARATION---------------------------------------
unsigned long uptime = 0;
//---------------------------------NETWORK DECLARATION--------------------------------------

char* wifi_cl_ssid, wifi_cl_pass, wifi_ap_ssid, wifi_ap_pass;
char wifi_mode;

//---------------------------------MQTT DECLARATION-----------------------------------------
//---------------------------------WEBSOCKET DECLARATION------------------------------------

AsyncWebServer server(HTTP_PORT);
AsyncWebSocket ws("/ws");


#include "helper.esp"
#include "configurator.esp"
#include "networks.esp"
#include "mqtt.esp"
#include "websocket.esp"


void ICACHE_FLASH_ATTR setup(){
  Serial.begin(115200);
  Serial.print(System_name);
}

void IRAM_ATTR loop(){
  uptime = millis();
}