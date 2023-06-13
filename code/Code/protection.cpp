#include <config.h>
#include <Arduino.h>

void protect_mist_fan(void *parameters)
{
    while (1)
    {
        if (!spray_enable)
        {
            vTaskDelay(time_for_fan_mist_protection / portTICK_PERIOD_MS);
            protection_fan_mist_enable = true;
            vTaskDelay(time_for_fan_mist_protection / portTICK_PERIOD_MS);
            protection_fan_mist_enable = false;
        }
    }
}
