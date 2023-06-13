#include <config.h>
#include <Arduino.h>
#include "led.h"
#include <stdint.h>

static uint32_t led_pin = 1;
static uint32_t led_pin_2 = 2;

int timeout = 120; // seconds to run for

// declare ssid and password pointers to store their values
char ssid[64];
char password[64];


// set NTP
const char *NTP_SERVER = "ch.pool.ntp.org";
const char *TZ_INFO = "EET-2EEST-3,M3.5.0/03:00:00,M10.5.0/04:00:00"; // enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)

tm timeinfo;
time_t now;
long unsigned lastNTPtime;
unsigned long lastEntryTime;

static const BaseType_t con_cpu = 0; // Core 0 will be the core for connections
static const BaseType_t app_cpu = 1; // Core 1 will be the core for functionality

WiFiClient client;
WiFiManager wm;

int maximum_reconnection_efforts = 5; // 20 times the ESP will try to recconect

// PcHarv Channel # 1
//  ####################################################################################
const long CHANNEL = 2174522;               // PcHarv Channel ID
const char *WRITE_API = "VFXDZNE1MBWLNI12"; // PcHarv Channel Write API

// Relay Remote Control
const long CHANNEL_FEEDBACK = 2174525;     // CHANNEL_FEEDBACK Channel ID
const char *READ_API = "GHMIUGKFQYOGGVG0"; // CHANNEL_FEEDBACK Channel Read API
// ####################################################################################

///////////////////////////////////////// Variables and Constants /////////////////////////////////////////

// Soil Hum limit values
// must be calibrated (see here https://wiki.dfrobot.com/Capacitive_Soil_Moisture_Sensor_SKU_SEN0193)
int soil_0_hum = 3060;   // is correct???
int soil_100_hum = 1380; // is correct???
bool connectionIsAlived = false;
int value;
long time_elapsed_cloud = 0;
float air_humidity, soil_humidity, air_temperature, water_level;
int height;
String ControlStatus = "";
float humid_max, humid_min, humid_soil;
float humid_max_prev, humid_min_prev, humid_soil_prev;

int counter_check_connection = 0;
int counter_start_watering = 0;
int counter_fan_delay = 0;
int interrupt_counter = 0;
// int counter_for_restart = 120; // every 2 hours the ESP will be restarted
int counter_for_hour_check = HOUR_CHECK;

bool spray_enable = false;
bool water_enable = false;
bool fan_enable = false;
bool lights_enable = false;
bool protection_fan_mist_enable = false;
bool update_display_enable = true;
bool heater_enable = false;
bool heater_fan_enable = false;
const int update_lcd_time = 2000;
const int read_sensor_time = 3000;
const int upload_data_cloud = 20000;
const int time_generic = 60000;
const int download_data_cloud = 15000;
const int check_connection_wifi = 18000;
const int run_automation_time = 500;
const int time_for_fan_mist_protection = 60000;
unsigned long lastTime_cloud = 0;
unsigned long lastTime_automation = 0;
unsigned long lastTime_generic = 0;

void setup()
{
  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // burnout will be disabled

  // put your setup code here, to run once:
  Serial.begin(115200);
  // pinMode(TRIGGER_PIN, INPUT_PULLUP);
  // init and get the time
  configTime(0, 0, NTP_SERVER);
  setenv("TZ", TZ_INFO, 1);

  // Pin Initialization
  pinMode(WATER_RELAY, OUTPUT);
  digitalWrite(WATER_RELAY, HIGH);

  pinMode(SPRAY_RELAY, OUTPUT);
  digitalWrite(SPRAY_RELAY, HIGH);

  pinMode(AIR_RELAY, OUTPUT);
  digitalWrite(AIR_RELAY, HIGH);

  // pinMode(IR_SENSOR_5CM, INPUT);
  pinMode(IR_SENSOR_10CM, INPUT);

  pinMode(WATER_SENSOR, INPUT_PULLDOWN);
  //  pinMode(LIGHTS1, OUTPUT);
  pinMode(LIGHTS, OUTPUT);

  pinMode(USER_INPUT, INPUT_PULLUP);
  pinMode(USER_INPUT_LED, INPUT);

  pinMode(HEATER, OUTPUT);
  digitalWrite(HEATER, HIGH);

  pinMode(HEATER_FAN, OUTPUT);
  digitalWrite(HEATER_FAN, HIGH);

  pinMode(FAN_MIST, OUTPUT);
  digitalWrite(FAN_MIST, HIGH);

  Serial.println("Waiting for connection to WiFi");

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.setSleep(false);
  if ((strlen(wm.getWiFiSSID(true).c_str()) == 0) || (digitalRead(USER_INPUT) == LOW))
  {
    manage_WiFi();
  }
  else
  {
    Serial.println(wm.getWiFiSSID(true).c_str());
    Serial.println(wm.getWiFiPass(true).c_str());
    setup_wifi();
  }

  Serial.print("WIFI status = ");
  Serial.println(WiFi.getMode());
  Serial.print("WIFI MAC Address = ");
  Serial.println(String(WiFi.macAddress()));
  ThingSpeak.begin(client); // Initialize ThingSpeak

  // lastNTPtime = time(&now);
  // lastEntryTime = millis();

  while (1)
  {
    delay(10000);
    if (!getLocalTime(&timeinfo))
    {
      Serial.println("Failed to obtain time");
    }
    else
    {
      showTime(timeinfo);
      break;
    }
  }
  manage_lights();

  // Humidity Sensor Ininialization
  // sht31.begin(0x44);
  Wire.begin();

}
void loop()
{
  Serial.println("#############################################################");

  run_automation();
  
  if ((millis() - lastTime_cloud) > upload_data_cloud)
  {
    check_WiFi_connection();
    send_values_to_cloud();
    lastTime_cloud = millis();
  }
  if ((millis() - lastTime_generic) > time_generic)
  {
    counter_for_hour_check--;
    lastTime_generic = millis();
  }

  if (counter_for_hour_check == 0)
  {
    if (!getLocalTime(&timeinfo))
    {
      Serial.println("Failed to obtain time");
    }
    else
    {
      showTime(timeinfo);
      counter_for_hour_check = HOUR_CHECK;
    }
  }
  Serial.println("#############################################################\n\n");
}