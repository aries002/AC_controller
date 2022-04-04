
#include "config.esp"

//---------------------------HARD WIRED VARIABLES--------------------------------------

#ifndef STATUS_PIN
#define STATUS_PIN 2
#endif

#ifndef TOKEN
#define TOKEN "1234567"
#endif
#ifndef DEVICE_NAME
#define DEVICE_NAME "AC_Controller"
#endif

#ifndef SERVER_HOST
#define SERVER_HOST "iot.umsida.ac.id"
#endif

#ifndef SENSOR_UPDATE_PATH
#define SENSOR_UPDATE_PATH "dev/api/"
#endif

#ifndef HTTP_PORT
#define HTTP_PORT 8080
#endif

#ifndef API_PATH
#define API_PATH "/api"
#endif

#ifndef API_DATA_TYPE
#define API_DATA_TYPE "application/json"
#endif


#ifndef AC_CONTROLLER_CHECK_INTERVAL
#define AC_CONTROLLER_CHECK_INTERVAL 5000
#endif

#ifndef AC_READ_TIMEOUT
#define AC_READ_TIMEOUT 4000
#endif

#ifndef SENSOR_UPDATE_INTERVAL
#define SENSOR_UPDATE_INTERVAL 5000
#endif
//---------------------------------SYSTEM VARIABLES---------------------------------------
unsigned long uptime = 0;
//system stat
bool reboot = false;
bool reset = false;
bool reconfig = false;
bool config = false;
bool debug = true;
bool updating = false;
bool allow_remote_power = false; //cinfig

char *token;
String device_name;
char *device_id;
int status_pin = STATUS_PIN;

String update_url;                                //firmware update source

String server_token = SERVER_TOKEN;
String server_host = SERVER_HOST;
// char *server_finger_print = SERVER_FINGERPRINT;
// String sensor_update_path = SENSOR_UPDATE_PATH;


//---------------------------------SENSOR VARIABLES---------------------------------------
bool sensor_ac_status = false;
// bool sensor_ac_command = false;
float sensor_room_temp;
float sensor_room_humid;
float sensor_listrik_voltage;
float sensor_listrik_power;
float sensor_listrik_current;
float sensor_listrik_pf;
float sensor_listrik_freq;
float sensor_listrik_kwh;




//---------------------------------NETWORK VARIABLES--------------------------------------

char* wifi_ssid;
char* wifi_pass;
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

//---------------------------------SETUP LIBRARY--------------------------------------------


void ICACHE_FLASH_ATTR def_var_loader(void){
    String tmp_token = TOKEN;
    token = strdup(tmp_token.c_str());

    String tmp_wifi_ssid = WIFI_SSID;
    wifi_ssid = strdup(tmp_wifi_ssid.c_str());

    String tmp_wifi_pass = WIFI_PASS;
    wifi_pass = strdup(tmp_wifi_pass.c_str());


}