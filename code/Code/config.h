#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <string.h>
#include <Wire.h>
#include <ThingSpeak.h>
#include "xbm.h"      // Wifi icon
#include <RTC.h>
#include <time.h>
#include <FreeRTOSConfig.h>
#include <Adafruit_Sensor.h>
#include <soc/soc.h>
#include <soc/rtc.h>

extern int timeout;

///////////////////////////////////////// Definitions /////////////////////////////////////////

// Connections of TFT 128X160
//////////////////////////////////////////////////////////////////////////////////////////////
/*
  DIN 23
  CLK 18
  CS  15  // Chip select control pin
  D/C 2  // Data Command control pin
  RST 9(ΕΝ)  // Reset pin (could connect to RST pin)
  //////////////////////////////////////////////////////////////////////////////////////////////*/

#define SOIL_HUM 36    // ok //36 // soil sensor
#define WATER_RELAY 33 // ok // water pumpr
#define SPRAY_RELAY 13 // ok // mist maker
#define AIR_RELAY 17   // ok // fan
//#define IR_SENSOR_5CM 25
#define IR_SENSOR_10CM 32 // height sensor
#define WATER_SENSOR 5   // water level sensor
#define FAN_MIST 25       // 19 // fan mist actuator
#define LIGHTS 26         // lights actuator
#define USER_INPUT 27    // user input --> interrupt
#define USER_INPUT_LED -1    // user input --> interrupt
#define HEATER 14 
#define HEATER_FAN 16 
#define HOUR_CHECK 5 // every 5 min

#define address 0x40


// set NTP
extern const char *ntpServer;
extern const long gmtOffset_sec;
extern const int daylightOffset_sec;
extern tm timeinfo;

extern WiFiClient client;
extern WiFiManager wm;

// Microgreen Channel # 1
//  ####################################################################################
extern const long CHANNEL;
extern const char *WRITE_API;

// Relay Remote Control
extern const long CHANNEL_FEEDBACK;
extern const char *READ_API;
// ####################################################################################

///////////////////////////////////////// Initialize counter /////////////////////////////////////////
extern volatile int count; // Trigger

///////////////////////////////////////// Header Functions /////////////////////////////////////////
// void setup_wifi(bool *connectionStatus);
void microAutomation(void);
float readAirTemp(void);
float readAirHum(void);
float readSoilHum(void);
float readWaterSensor(void);
int readHeight(void);
void setup_wifi(void);
void manage_WiFi(void);
void printLocalTime(void);
void print_display(void *parameters);
void update_display(void);
// void update_sensor_values(void *parameters);
void update_sensor_values(void);
void send_values_to_cloud();
void receive_values_from_cloud();
void run_automation(void);
void check_WiFi_connection(void);
void IRAM_ATTR onTime(void);
bool getNTPtime(int sec);
void showTime(tm localTime);
void water_pump_soil(void *parameters);
void protect_mist_fan(void *parameters);
void createWaterPumpSoilTask(void);
void deleteWaterPumpSoilTask(void);
void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info);
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
void manage_lights(void);
void IRAM_ATTR userButtonPressed(void);
uint8_t readReg(uint8_t reg, const void* pBuf, size_t size);
///////////////////////////////////////// Variables and Constants /////////////////////////////////////////

extern int soil_0_hum;
extern int soil_100_hum;
extern bool connectionIsAlived;
extern int value;
extern long time_elapsed_cloud;
extern float air_humidity, soil_humidity, air_temperature, water_level;
extern int height;
extern String ControlStatus;
extern float humid_max, humid_min, humid_soil;
extern float humid_max_prev, humid_min_prev, humid_soil_prev;
extern int counter_check_connection;
extern int counter_spray;
extern int counter_water;
extern int interrupt_counter;
extern int counter_for_restart;

extern bool spray_enable;
extern bool water_enable;
extern bool fan_enable;
extern bool lights_enable;
extern bool protection_fan_mist_enable;
extern bool update_display_enable;
extern bool heater_enable;
extern bool heater_fan_enable;

extern const int update_lcd_time;
extern const int read_sensor_time;
extern const int upload_data_cloud;
extern const int download_data_cloud;
extern const int check_connection_wifi;
extern const int run_automation_time;
extern const char *NTP_SERVER;
extern const char *TZ_INFO;
extern tm timeinfo;
extern time_t now;
extern long unsigned lastNTPtime;
extern unsigned long lastEntryTime;
extern int maximum_reconnection_efforts;
extern int counter_start_watering;
extern int counter_fan_delay;
extern const int time_for_fan_mist_protection;