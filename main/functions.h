#ifndef FUNCTIONS_H

#define FUNCTIONS_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define NVS_WLAN_SSID "wlan.ssid"
#define NVS_WLAN_PSK "wlan.psk"
#define NVS_TOTP_KEY "totp.key"
#define NTP_SERVER_1 "ntp.aliyun.com"
#define NTP_SERVER_2 "ntp.tuna.tsinghua.edu.cn"
#define NTP_SERVER_3 "time1.cloud.tencent.com"
#define NTP_RETRY_COUNT 20

void gpioSetHigh(gpio_num_t);
void gpioSetLow(gpio_num_t);
void printLocalTime();
void ntpUpdate();

#endif