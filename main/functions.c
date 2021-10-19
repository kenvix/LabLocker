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

void gpioSetHigh(gpio_num_t pin)
{
    gpio_set_level(pin, 1);
}

void gpioSetLow(gpio_num_t pin)
{
    gpio_set_level(pin, 0);
}

void ntpUpdate() {
    ntp_init();
}