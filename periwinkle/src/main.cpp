#define VERSION 0.2
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
// #include <WiFiClientSecureBearSSL.h>

#include <LittleFS.h>

#include <PZEM004Tv30.h>
#include <Wire.h>
#include "SparkFun_SHTC3.h"


#include "variables.h"
#include "helper.esp"
#include "system.esp"
#include "sensor.esp"
#include "webserver.esp"

void ICACHE_FLASH_ATTR setup(){
  //serial setup
  def_var_loader();
  Serial.begin(115200);
  delay(200);
  Serial.println();


  //sensor initialize
  sensor_setup();

//start file system
  if(!LittleFS.begin()){
    Log("Unable to mount File System",0);
    Log("Formating file system",1);
    LittleFS.format();
    Serial.println("REBOOTING IN");
    for (uint8_t t = 5; t > 0; t--) {
      Serial.printf("%d...\n", t);
      Serial.flush();
      delay(1000);
    }
    ESP.restart();
  }

  //start load configuration
  
  // generate device id and device name
  device_id = strdup(gen_device_id().c_str());
  device_name = gen_device_name();

  test_config();
  //load configuration
  // bool sts_sys_conf = true;// for debuging remove after finish
  // bool sts_net_conf = true;//same
  bool sts_sys_conf = config_load_system();
  bool sts_net_conf = config_load_network();
  //print device info
  Log("Device id = " + String(device_id),2);
  Log("Device name = " + device_name,2);
  
  WiFi.hostname(strdup(device_name.c_str()));
  //check if configuration is success
  if(sts_sys_conf && sts_net_conf){
    config = true;
    //check if network configuration is success then start network
    if(wifi_mode){
      if(!wifi_cl_connect(wifi_ssid,wifi_pass)){
        wifi_ap_setup(strdup(device_name.c_str())); //start failsafe ap connection
      }
    }
    else{
      wifi_ap_setup(wifi_ssid,wifi_pass);
    }
    webserver();
  }
  else{
    Log("Configuration failed. Starting failsafe");
    // default setup or fail safe configuration
    wifi_ap_setup(strdup(device_name.c_str()));
    failsafe_webserver();
  }
}

void IRAM_ATTR loop(){
  net_status = (WiFiMulti.run() == WL_CONNECTED) ? true : false;
  //get system clock and uptime
  uptime = millis();
  //reboot the system if required
  if(reset){
    reset_config();
  }
  if(reboot){
    Log("Device rebooting...",2);
    ESP.restart();
  }

  if(reconfig){
    Log("Updating config");
    reconfig = false;
  }

  system_loop();
  if(config){
    wifi_loop();
    sensor_loop();
  }
  if(updating){
    do_update();
  }
  if(!updating){
    //keep the system alive
    breathe();
  }

}