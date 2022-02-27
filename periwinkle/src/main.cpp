#define VERSION 0.01
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

#include "config.esp"
//---------------------------------SYSTEM VARIABLES---------------------------------------
unsigned long uptime = 0;
//system stat
bool reboot = false;
bool reset = false;
bool reconfig = false;
bool config = false;
bool debug = true;
bool updating = false;

char *token = TOKEN;
char *device_name;
char *device_id;
int status_pin = STATUS_PIN;

String update_url;                                //firmware update source

String server_token = SERVER_TOKEN;
String server_host = SERVER_HOST;
// char *server_finger_print = SERVER_FINGERPRINT;
// String sensor_update_path = SENSOR_UPDATE_PATH;


//---------------------------------SENSOR VARIABLES---------------------------------------
bool sensor_ac_status = false;
bool sensor_ac_command = false;
float sensor_room_temp;
float sensor_room_humid;
float sensor_listrik_voltage;
float sensor_listrik_power;
float sensor_listrik_current;
float sensor_listrik_pf;
float sensor_listrik_freq;




//---------------------------------NETWORK VARIABLES--------------------------------------

char* wifi_ssid = WIFI_SSID;
char* wifi_pass = WIFI_PASS;
bool wifi_mode = WIFI_MODE;
bool net_status = false;

//---------------------------------CONTROLLER VARIABLES-----------------------------------

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

WiFiClient wifi_client;
// WiFiClientSecure secure_wifi_client;
ESP8266WiFiMulti WiFiMulti;

#include "helper.esp"
#include "networks.esp"
#include "system.esp"
#include "sensor.esp"
#include "webserver.esp"

void ICACHE_FLASH_ATTR setup(){
  //serial setup
  Serial.begin(115200);
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
  device_name = strdup(gen_device_name().c_str());
  test_config();
  //load configuration
  bool sts_sys_conf = true;// for debuging remove after finish
  bool sts_net_conf = true;//same
  // bool sts_sys_conf = config_load_system();
  // bool sts_net_conf = config_load_network();
  //print device info
  Log("Device id = " + String(device_id),2);
  Log("Device name = " + String(device_name),2);
  
  //check if configuration is success
  if(sts_sys_conf && sts_net_conf){
    config = true;
    //check if network configuration is success then start network
    if(wifi_mode){
      if(!wifi_cl_connect(wifi_ssid,wifi_pass)){
        wifi_ap_setup(); //start failsafe ap connection
      }
    }
    else{
      wifi_ap_setup(wifi_ssid,wifi_pass);
    }
    webserver();
  }
  else{
    // default setup or fail safe configuration
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