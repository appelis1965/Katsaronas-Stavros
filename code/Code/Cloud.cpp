#include <config.h>
#include <Arduino.h>

void send_values_to_cloud()
{

    // Serial.println("Upload data to Cloud");
    int statusCode = 0;
    // Set the fields with the values
    ThingSpeak.setField(1, air_temperature);
    ThingSpeak.setField(2, air_humidity);
    ThingSpeak.setField(3, soil_humidity);
    ThingSpeak.setField(4, height);
    ThingSpeak.setField(5, water_level);
    ThingSpeak.setField(6, lights_enable);

    // Write to the ThingSpeak channel
    statusCode = ThingSpeak.writeFields(CHANNEL, WRITE_API);
    if (statusCode == 200)
    {
        Serial.println("Data upload successful.");
        connectionIsAlived = true;
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    else
    {
        Serial.println("Problem updating MicroGreen channel. HTTP error code " + String(statusCode));
        connectionIsAlived = false;
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void receive_values_from_cloud()
{

    int statusCode = 0;

    // Read Remote Control Channel from ThingSpeak
    //  Read in field 4 (all status of Remote Control in String) of the private channel which is the Remote Control Status
    ControlStatus = ThingSpeak.readStringField(CHANNEL_FEEDBACK, 4, READ_API);

    // Check the status of the read operation to see if it was successful
    statusCode = ThingSpeak.getLastReadStatus();
    if (statusCode == 200)
    {
        Serial.println("Control Channel read successful.");
        connectionIsAlived = true;
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    else
    {
        Serial.println("Problem reading feedback data. HTTP error code " + String(statusCode));
        connectionIsAlived = false;
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}