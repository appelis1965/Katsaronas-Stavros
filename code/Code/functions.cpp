#include <config.h>
#include <Arduino.h>

void printLocalTime()
{
  getNTPtime(10);
  showTime(timeinfo);
}

bool getNTPtime(int sec)
{
  {
    uint32_t start = millis();
    do
    {
      time(&now);
      localtime_r(&now, &timeinfo);
      delay(10);
    } while (((millis() - start) <= (1000 * sec)) && (timeinfo.tm_year < (2016 - 1900)));
    if (timeinfo.tm_year <= (2016 - 1900))
      return false; // the NTP call was not successful
    // Serial.print("now ");
    // Serial.println(now);
    char time_output[30];
    strftime(time_output, 30, "%a  %d-%m-%y %T", localtime(&now));
    memset(time_output, 0, 30); // clear char

    // Serial.println(time_output);
    // Serial.println();
  }
  return true;
}

void showTime(tm localTime)
{
  Serial.print(localTime.tm_mday);
  Serial.print('/');
  Serial.print(localTime.tm_mon + 1);
  Serial.print('/');
  Serial.print(localTime.tm_year - 100);
  Serial.print('-');
  Serial.print(localTime.tm_hour);
  Serial.print(':');
  Serial.print(localTime.tm_min);
  Serial.print(':');
  Serial.print(localTime.tm_sec);
  Serial.print(" Day of Week ");
  if (localTime.tm_wday == 0)
    Serial.println(7);
  else
    Serial.println(localTime.tm_wday);
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Connected to AP successfully!");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  vTaskSuspendAll();
  delay(1000);
  ESP.restart();
}

void manage_lights()
{
  if ((timeinfo.tm_hour <= 18) && (timeinfo.tm_hour >= 7)) // Lights will be open from 7 to 19 hours (18:59)
  {
    // Serial.println("Lights ON");
    digitalWrite(LIGHTS, LOW);
    lights_enable = true;
  }
  else
  {
    // Serial.println("Lights OFF");
    digitalWrite(LIGHTS, HIGH);
    lights_enable = false;
  }
}

uint8_t readReg(uint8_t reg, const void* pBuf, size_t size)
{
  if (pBuf == NULL) {
    Serial.println("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  Wire.beginTransmission((uint8_t) address);
  Wire.write(&reg, 1);
  if ( Wire.endTransmission() != 0) {
    return 0;
  }
  delay(20);
  Wire.requestFrom((uint8_t) address, (uint8_t) size);
  for (uint16_t i = 0; i < size; i++) {
    _pBuf[i] = Wire.read();
  }
  return size;
}
