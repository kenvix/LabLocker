#ifndef FUNCTIONS_H

#define FUNCTIONS_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
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

#define NVS_WLAN_SSID "wlan.ssid"
#define NVS_WLAN_PSK "wlan.psk"
#define NVS_TOTP_KEY "totp.key"
#define NTP_SERVER_1 "ntp.aliyun.com"
#define NTP_SERVER_2 "ntp.tuna.tsinghua.edu.cn"
#define NTP_SERVER_3 "time1.cloud.tencent.com"
#define NTP_RETRY_COUNT 20

typedef struct SystemStatus
{
    unsigned char isWlanConnected : 1;
    unsigned char isNtpCreated : 1;
} SystemStatus;

extern SystemStatus systemStatus;

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
extern const int CONNECTED_BIT;
extern const int ESPTOUCH_DONE_BIT;

/* The examples use WiFi configuration that you can set via project configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/

extern esp_mqtt_client_handle_t mqtt_client;

/* FreeRTOS event group to signal when we are connected*/
extern EventGroupHandle_t s_wifi_event_group;

void gpioSetHigh(gpio_num_t);
void gpioSetLow(gpio_num_t);
void printLocalTime();
void ntpUpdate();

#endif