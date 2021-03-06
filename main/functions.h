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
#include "nvs.h"

#define NVS_WLAN_SSID "wlan.ssid"
#define NVS_WLAN_PSK "wlan.psk"
#define NVS_TOTP_KEY "totp.key"

#define PIN_DOOR_OPEN 19
#define PIN_DOOR_CLOSE 22
#define PIN_LED_SYS 14
#define PIN_LED_WLAN 26
#define PIN_LED_NTP  25
#define PIN_LED_MQTT 16
#define PIN_BEEP 33
#define PIN_WLAN_RESET 32

#define DOOR_ROLLTATE_DELAY 400
#define DOOR_WAIT_USER_DELAY 8000

#define NTP_SERVER_1 "ntp.aliyun.com"
#define NTP_SERVER_2 "ntp.tuna.tsinghua.edu.cn"
#define NTP_SERVER_3 "time1.cloud.tencent.com"
#define NTP_RETRY_COUNT 20

typedef struct SystemStatus
{
    unsigned int isWlanInited : 1;
    unsigned int isWlanSmartConfigRunning : 1;
    unsigned int isWlanConnected : 1;
    unsigned int isNtpCreated : 1;
    unsigned int isNtpFinished : 1;
    unsigned int isBluetoothInited : 1;
    unsigned int isBluFiRunning : 1;
    
    unsigned int isWlanLedBlinking : 1;
    unsigned int isMqttLedBlinking : 1;
    unsigned int isNtpLedBlinking : 1;
    unsigned int isSysLedBlinking : 1;

} SystemStatus;

extern volatile SystemStatus systemStatus;

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
extern nvs_handle nvs;
extern char* hostname;

void gpioSetHigh(gpio_num_t);
void gpioSetLow(gpio_num_t);
void printLocalTime();
void ntpUpdate();



typedef void (* GPIOFunction)();

void gpioDoorOpen();
void gpioDoorClose();
void initSuccess();
void gpioAsync(GPIOFunction f);
void gpioDoorUnlock();
void gpioInit();
void gpioBlinkNtp();
void gpioBlinkWlan();
void gpioBlinkMqtt();
void gpioBeepOnce();

#endif