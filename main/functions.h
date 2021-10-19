#ifndef FUNCTIONS_H

#define FUNCTIONS_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define NVS_WLAN_SSID "wlan.ssid"
#define NVS_WLAN_PSK "wlan.psk"
#define NVS_TOTP_KEY "totp.key"
#define NTP_SERVER "ntp.aliyun.com"
#define NTP_SERVER_ALT "time1.cloud.tencent.com"

void gpioSetHigh(gpio_num_t);
void gpioSetLow(gpio_num_t);
void printLocalTime();
void ntpUpdate();

#endif