#include "functions.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <lwip/apps/sntp.h>
#include <stdarg.h>
#include <stdlib.h>
#include "sntp.h"

static const char* TAG = "Functions";

void gpioSetHigh(gpio_num_t pin)
{
    gpio_set_level(pin, 1);
}

void gpioSetLow(gpio_num_t pin)
{
    gpio_set_level(pin, 0);
}

void ntpUpdate() 
{
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    if (timeinfo.tm_year < (2016 - 1900))
    {
        ESP_LOGI(TAG, "Running SNTP Client");
        ntp_init();
        ntpUpdate();
        systemStatus.isNtpFinished = 1;
    }
}