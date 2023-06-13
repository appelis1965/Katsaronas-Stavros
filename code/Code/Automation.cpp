#include <config.h>
#include <Arduino.h>

void run_automation()
{
  // showTime(timeinfo);
  // Serial.println("Run Automation");
  vTaskDelay(run_automation_time / portTICK_PERIOD_MS);
  Serial.println("Reading sensor values");
  update_sensor_values();
  vTaskDelay(run_automation_time / portTICK_PERIOD_MS);
  // Serial.println("Running Automation");
  microAutomation();
}
void microAutomation()
{
  // setting the temperature inside the tower
  if (air_temperature < 17)
  {
    heater_enable = true;
    heater_fan_enable = true;
    spray_enable = false;
    fan_enable = false;
  }
  else if (air_temperature >= 17 && air_temperature < 20)
  {
    // heater_enable = false;
    if (air_humidity < 45)
    {
      heater_enable = false;
      heater_fan_enable = false;
      spray_enable = true;
      fan_enable = false;
    }
    else if (air_humidity >= 45 && air_humidity <= 60)
    {
      heater_enable = false;
      heater_fan_enable = false;
      spray_enable = false;
      fan_enable = false;
    }
    else if (air_humidity > 60)
    {
      heater_enable = false;
      heater_fan_enable = false;
      spray_enable = false;
      fan_enable = true;
    }
  }
  else if (air_temperature >= 20 && air_temperature < 25)
  {
    // heater_enable = false;
    if (air_humidity < 60)
    {
      heater_enable = false;
      heater_fan_enable = false;
      spray_enable = true;
      fan_enable = false;
    }
    else if (air_humidity >= 60 && air_humidity <= 75)
    {
      heater_enable = false;
      heater_fan_enable = false;
      spray_enable = false;
      fan_enable = false;
    }
    else if (air_humidity > 75)
    {
      heater_enable = false;
      heater_fan_enable = false;
      spray_enable = false;
      fan_enable = true;
    }
  }
  else if (air_temperature >= 25 && air_temperature < 30)
  {
    // heater_enable = false;
    if (air_humidity < 75)
    {
      heater_enable = false;
      heater_fan_enable = false;
      spray_enable = true;
      fan_enable = false;
    }
    else if (air_humidity >= 75 && air_humidity <= 80)
    {
      heater_enable = false;
      heater_fan_enable = false;
      spray_enable = false;
      fan_enable = false;
    }
    else if (air_humidity > 80)
    {
      heater_enable = false;
      heater_fan_enable = false;
      spray_enable = false;
      fan_enable = true;
    }
  }
  else if (air_temperature > 30)
  {
    heater_enable = false;
    heater_fan_enable = true;
    spray_enable = false;
    fan_enable = true;
  }
  // Setting Soil Humidity
  if (soil_humidity < 70)
  {
    Serial.println("Watering ...");
    water_enable = true;
  }
  else if (soil_humidity > 90)
  {
    Serial.println("Not Watering ...");
    water_enable = false;
  }
  if (spray_enable == true)
  {
    Serial.println("Started Spraying...");
    digitalWrite(SPRAY_RELAY, LOW);
    delay(2000);
    digitalWrite(FAN_MIST, LOW);
  }
  else
  {
    digitalWrite(SPRAY_RELAY, HIGH);
    delay(2000);
    digitalWrite(FAN_MIST, HIGH);
    Serial.println("Stopped Spraying.");
  }
  if (fan_enable == true)
  {
    Serial.println("Started Fan...");
    digitalWrite(AIR_RELAY, LOW);
  }
  else
  {
    digitalWrite(AIR_RELAY, HIGH);
    Serial.println("Stopped Fan.");
  }
  if (heater_enable == true)
  {
    Serial.println("Started Heater...");
    digitalWrite(HEATER, LOW);
  }
  else
  {
    Serial.println("Stoped Heater...");
    digitalWrite(HEATER, HIGH);
  }
  if (heater_fan_enable == true)
  {
    Serial.println("Started Heater FAN...");
    digitalWrite(HEATER_FAN, LOW);
  }
  else
  {
    Serial.println("Stoped Heater FAN...");
    digitalWrite(HEATER_FAN, HIGH);
  }
  manage_lights();
}