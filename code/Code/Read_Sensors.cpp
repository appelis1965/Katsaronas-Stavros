#include <config.h>
#include <Arduino.h>

// void update_sensor_values(void *parameters)
void update_sensor_values(void)
{
  // while (1)
  //{
  // Serial.println("Read sensors");
  // vTaskDelay(read_sensor_time / portTICK_PERIOD_MS);
  float air_humidity_previous_value = air_humidity;
  float air_temperature_previous_value = air_temperature;
  air_humidity = readAirHum();
  soil_humidity = readSoilHum();
  air_temperature = readAirTemp();
  height = readHeight();
  water_level = readWaterSensor();
}

float readWaterSensor(void)
{
  int val;
  if (digitalRead(WATER_SENSOR) == LOW)
  {
    val = 0;
    Serial.println("level empty");
  }
  else
  {
    val = 1;
    Serial.println("level full");
  }
  return val;
}

int readHeight(void)
{
  int r1, r2, h;
  // r1 = digitalRead(IR_SENSOR_5CM);
  r2 = digitalRead(IR_SENSOR_10CM);
  // Serial.println(r1);

  /*
    if(r1 == LOW)
    Serial.println("5cm DETECT");
    else
    Serial.println("5cm CLEAR");

    if(r2 == LOW)
    Serial.println("10cm DETECT");
    else
    Serial.println("10cm CLEAR");
  */

  if (r2 == HIGH)
  {
    h = 0;
    //printf("Height %d cm\n", h);
  }
  /*else if ( (r1 == LOW) )
    {
    h = 5;
    }*/
  else
    h = 10;
  printf("Height %d cm\n", h);

  return h;
}

float readSoilHum(void)
{
  // delay(200);
  // float val = 40 + counter_for_testing_water_pump;
  // Serial.println(analogRead(SOIL_HUM));

  float val = map(analogRead(SOIL_HUM), soil_0_hum, soil_100_hum, 0, 100);
  if (val < 0)
    val = 0;
  else if (val > 100)
    val = 100;

  // delay(200);
  printf("Soil Hum %0.2f %\n", val);
  return val;
}

float readAirHum(void)
{

  //float h = sht31.readHumidity();
  uint8_t buf[4] = {0};
  uint16_t data;
  readReg(0x00, buf, 4);
  data = buf[2] << 8 | buf[3];
  float h =  ((float)data / 65535.0) * 100;
  if (isnan(h))
  {
    Serial.println("Failed to read from SHT31 sensor!");
    return air_humidity;
  }

  printf("Air Hum %0.2f %\n", h);
  return h;
}

// Sensors Read Functions
float readAirTemp(void)
{
  // Read temperature as Celsius
  //float t = sht31.readTemperature();
  // float t = 35;

  uint8_t buf[4] = {0};
  uint16_t data;
  readReg(0x00, buf, 4);
  data = buf[0] << 8 | buf[1];
  float t = ((float)data * 165 / 65535.0) - 40.0;

  if (isnan(t))
  {
    // Serial.println("Failed to read from SHT31 sensor!");
    return air_temperature;
  }
  printf("Air Temp %0.2f oC\n", t);
  return t;
}