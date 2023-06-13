#include <Arduino.h>
#include <config.h>

void water_pump_soil(void *parameters)
{
    while (1)
    {
        if (humid_soil != 0)
        {

            // printf("Water pump started ..\r\n");
            digitalWrite(WATER_RELAY, LOW);

            vTaskDelay(3000 / portTICK_PERIOD_MS);
            // printf("Water pump stopped ..\r\n");
            digitalWrite(WATER_RELAY, HIGH);

            vTaskDelay(5000 / portTICK_PERIOD_MS);
        }
    }
}