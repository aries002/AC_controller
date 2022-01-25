#define VERSION 0.01
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <LittleFS.h>
#include <PubSubClient.h>
// #include "DHT.h"
#include <PZEM004Tv30.h>
#include <Wire.h>
// #include "Adafruit_SHTC3.h"



#include "config.esp"
//---------------------------------SYSTEM VARIABLES---------------------------------------
unsigned long uptime = 0;
bool reboot = false;
bool reset = false;
bool config = false;
char *token = TOKEN;
char *device_name;
char *device_id;
bool debug = true;
int status_pin = STATUS_PIN;
bool updating = false;
String update_url;

//---------------------------------SENSOR VARIABLES---------------------------------------
bool sensor_ac_status = false;
bool sensor_ac_command = false;
unsigned long sensor_last_update = 0;
unsigned long sensor_interval_update = SENSOR_UPDATE_INTERVAL;
unsigned long sensor_data_sender_interval = SENSOR_UPDATE_INTERVAL;


//---------------------------------NETWORK VARIABLES--------------------------------------
// char* wifi_cl_ssid = "wireless@umsida.ac.id";
// char* wifi_cl_pass = "AhmadDahlan1868";
// char* wifi_ap_ssid = "Almost Smart";
// char* wifi_ap_pass = NULL;
char* wifi_ssid = WIFI_SSID;
char* wifi_pass = WIFI_PASS;
// char* wifi_mode = "c";
bool wifi_mode = WIFI_MODE;
bool net_status = false;

//---------------------------------CONTROLLER VARIABLES-----------------------------------
char *controller_token;
char *controller_host;
char *sensor_update_path;

unsigned long controller_last_update = 0;

//---------------------------------MQTT VARIABLES-----------------------------------------
// char *mqtt_broker_ip;
// char *mqtt_topic_sub;
// char *mqtt_topic_pub;
// char *mqtt_client_id;
// char *mqtt_client_user;
// char *mqtt_client_pass;
// unsigned long mqtt_last_dc = 0;
// unsigned long mqtt_send_interval = 10000;
// unsigned long mqtt_send_last = 0;
// bool mqtt_login = false;
// bool mqtt_enable = false;

//---------------------------------WEBSOCKET VARIABLES------------------------------------


//---------------------------------SETUP LIBRARY--------------------------------------------
AsyncWebServer server(HTTP_PORT);
AsyncWebSocket ws(WEBSOCKET_PATH);


#include "helper.esp"
#include "ota_update.esp"
#include "sensor.esp"
#include "system.esp"
#include "networks.esp"
#include "webserver.esp"

void ICACHE_FLASH_ATTR setup(){
  //serial setup
  Serial.begin(115200);
  Serial.println();

  //sensor initialize
  sensor_setup();

  // dht.begin();


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
  device_name = strdup(gen_device_name().c_str());
  //load configuration
  bool sts_sys_conf = true;// for debuging remove after finish
  bool sts_net_conf = true;//same
  // bool sts_sys_conf = config_load_system();
  // bool sts_net_conf = config_load_network();
  //print device info
  Log("Device id = " + String(device_id),2);
  Log("Device name = " + String(device_name),2);
  //check if configuration is success
  if(sts_sys_conf){
    config = true;
    //check if network configuration is success then start network
    if(sts_net_conf){
      net_status = wifi_begin();
    }
    else{
      // if configuration failed or did't exist start AP
      wifi_ap_setup(device_name, WIFI_FAIL_PASS);
    }
  }
  // start web server
  webserver();
  if(config && net_status){

  }
  test_config();
}

void IRAM_ATTR loop(){
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

  if(config){
    //update sensor data
    update_sensor_ac();
    ac_controller();
    if(uptime - sensor_last_update >= sensor_interval_update){
      Log("updating sensor");
      update_sensor();
      //send sensor data to server
      if(net_status){
        sensor_data_sender();
      }
      sensor_last_update = uptime;
    }
  }
  if(updating){
    do_update();
  }
  if(!updating){
    //keep the system alive
    breathe();
  }
}