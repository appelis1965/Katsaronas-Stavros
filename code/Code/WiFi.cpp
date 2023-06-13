#include <config.h>
#include <Arduino.h>

void check_WiFi_connection()
{

  Serial.println("Checking WiFi connection");
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(wm.getWiFiSSID(true).c_str(), wm.getWiFiPass(true).c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi!");
  }
  else
  {
    Serial.println("WiFi is connected");
  }
}

void setup_wifi()
{
  // My code
  WiFi.begin(wm.getWiFiSSID(true).c_str(), wm.getWiFiPass(true).c_str());
  int counter = 0;
  int try_counter = 0;
  while (!WiFi.isConnected())
  {
    Serial.print(".");
    delay(500);
    counter++;
    if (counter == 20)
    {
      try_counter++;
      WiFi.begin(wm.getWiFiSSID(true).c_str(), wm.getWiFiPass(true).c_str());

      counter = 0;
      if (try_counter == 10)
      {
        int wait_counter = 2; // how many min will wait until starting again
        while (1)
        {
          wait_counter--;
          if (wait_counter == 0)
          {
            ESP.restart();
          }
        }
      }
    }
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());
}

void manage_WiFi()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_AP); // explicitly set mode, esp defaults to STA+AP
                      // is configuration portal requested?s`1w3as4 3
  // reset settings - for testing
  // wm.resetSettings();

  // set configportal timeout
  wm.setConfigPortalTimeout(timeout);

  if (!wm.startConfigPortal("PcHarv Tower"))
  {
    Serial.println("failed to connect and hit timeout");
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    // reset and try again, or maybe put it to deep sleep
    ESP.restart();
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
  interrupt_counter = 0;
}
