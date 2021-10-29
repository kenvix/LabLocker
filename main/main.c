#include "main.h"
#include "functions.h"
#include "matrix_keyboard.h"
#include "esp_smartconfig.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_wpa2.h"
#include "esp_event.h"
#include "smartconfig.h"
#include "blufi_example.h"
#include "time.h"
#include "keydata.h"
#include "ktotp.h"
#include "totp.h"

/* The examples use WiFi configuration that you can set via project configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/

esp_mqtt_client_handle_t mqtt_client;
nvs_handle nvs;

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

static const char* TAG = TAG_MAIN;
static char* channel_id;
volatile SystemStatus systemStatus;

static void wifiConnectAndWait() {
    bool selfBlink = false;
    if (systemStatus.isWlanLedBlinking == 0) {
        gpioAsync(gpioBlinkWlan);
        selfBlink = true;
    }

    esp_wifi_connect();
    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
            EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
            EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
        wifiConnectAndWait();
    }
    else
    {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

    if (selfBlink) {
        systemStatus.isWlanLedBlinking = 0;
    }
}

static void event_handler(void* arg, esp_event_base_t event_base,
    int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGI(TAG, "WIFI ASSOICATION LOST");
        wifiConnectAndWait();
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_GOT_IP6)
    {
        ip_event_got_ip6_t* event = (ip_event_got_ip6_t*)event_data;
        ESP_LOGI(TAG, "got ipv6:" IPSTR, IP2STR(&event->ip6_info.ip));
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE) {
        ESP_LOGI(TAG, "Scan done");
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL) {
        ESP_LOGI(TAG, "Found channel");
    }
}

void wifi_init_sta(wifi_config_t wifi_config)
{
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    gpioAsync(gpioBlinkWlan);
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &event_handler,
        NULL,
        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &event_handler,
        NULL,
        &instance_got_ip));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
        IP_EVENT_GOT_IP6,
        &event_handler,
        NULL,
        &instance_got_ip));
    ESP_ERROR_CHECK(esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));


    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
            EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
        systemStatus.isWlanLedBlinking = 0;
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
            EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
        wifiConnectAndWait();
    }
    else
    {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

    /* The event will not be processed after unregister */
}

char handleCommand(char* data, int data_len) {
    if (data_len == 0)
    {
        return 0;
    }
    else if (memcmp("open", data, data_len) == 0)
    {
        ESP_LOGI(TAG, "Door Control: OPEN and KEEP");
        gpioDoorOpen();
        return 0;
    }
    else if (memcmp("close", data, data_len) == 0)
    {
        ESP_LOGI(TAG, "Door Control: CLOSE and KEEP");
        gpioDoorClose();
        return 0;
    }
    else if (memcmp("unlock", data, data_len) == 0)
    {
        ESP_LOGI(TAG, "Door Control: UNLOCK: OPEN and CLOSE");
        gpioAsync(gpioDoorUnlock);
        return 0;
    }
    else if (memcmp("echo ", data, data_len) == 0)
    {
        int size = sizeof(char) * data_len - 4;
        char* buffer = (char*)malloc(size);
        memcpy(buffer, data + 5, size);
        buffer[size - 1] = 0;
        ESP_LOGI(TAG, "Echo data: %s", buffer);
        free(buffer);
        return 0;
    }
    else if (memcmp("set key ", data, data_len) == 0)
    {
        ESP_LOGI(TAG, "TOTP Control: Receiving new TOTP PSK");

        return 0;
    }
    else if (memcmp("get totp", data, data_len) == 0)
    {
        ESP_LOGI(TAG, "Current KTOTP key is %06u", ktotpGenerateToken(0));
        ESP_LOGI(TAG, "Previous KTOTP key is %06u", ktotpGenerateToken(-1));
        ESP_LOGI(TAG, "Next KTOTP key is %06u", ktotpGenerateToken(1));
        return 0;
    }
    else if (memcmp("date", data, data_len) == 0)
    {
        time_t now;
        struct tm timeinfo;
        char strftime_buf[64];
        ESP_LOGI(TAG, "The current UNIX TIME %ld", time(&now));

        localtime_r(&now, &timeinfo);
        strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);

        ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);

        strftime(strftime_buf, 26, "%Y-%m-%d %H:%M:%S", &timeinfo);
        ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);
        return 0;
    }
    else if (memcmp("reboot", data, data_len) == 0)
    {
        ESP_LOGI(TAG, "System control: reboot!");
        esp_restart();
        return 0;
    }
    else
    {
        ESP_LOGW(TAG, "Received unknown command: %s", data);
        return 1;
    }
}

esp_err_t mqtt_handle(esp_mqtt_event_handle_t event)
{
    if (event->event_id == MQTT_EVENT_CONNECTED)
    {
        int id = esp_mqtt_client_subscribe(mqtt_client, channel_id, 0);
        ESP_LOGI(TAG, "subscribe success %d", id);
        initSuccess();
    }

    if (event->event_id == MQTT_EVENT_DATA)
    {
        ESP_LOGI(TAG, "new message, size: %d", event->data_len);
        handleCommand(event->data, event->data_len);
    }
    return ESP_OK;
}

void init_mqtt()
{
    char* device_id = malloc(sizeof("ESP32-") + sizeof(CONFIG_CLIENT_ID));
    sprintf(device_id, "ESP32-%s", CONFIG_CLIENT_ID);

    //ESP_LOGI(TAG, "CLIENT_ID %s",device_id);
    const esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_MQTT_SERVER,
        .client_id = device_id,
        .event_handle = &mqtt_handle };
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);

    if (mqtt_client != NULL)
    {
        ESP_LOGI(TAG, "mqtt client create success");
        ESP_ERROR_CHECK(esp_mqtt_client_start(mqtt_client));
        //
    }
}

void wifiCheckReset() {
    int state = gpio_get_level(PIN_WLAN_RESET);
    ESP_LOGI(TAG, "WLAN reset pin in %d state", state);
    if (state == 1) {
        vTaskDelay(50);
        if (state == 1) {
            gpioAsync(gpioBeepOnce);
            ESP_LOGI(TAG, "WLAN reset pressed");
            ESP_LOGI(TAG, "WLAN reset cfg.wifi %d", nvs_erase_key(nvs, "cfg.wifi"));
            nvs_commit(nvs);
        }
    }
}

void init_all() {
    {
        // Initialize NVS/NVRAM
        esp_err_t ret = nvs_flash_init();
        if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
        {
            ESP_LOGI(TAG, "Earsing NVS/NVRAM ...");
            ESP_ERROR_CHECK(nvs_flash_erase());
            ret = nvs_flash_init();
        }
        ESP_ERROR_CHECK(ret);

        ESP_LOGI(TAG, "Initializing NVS/NVRAM ...");
        ESP_ERROR_CHECK(nvs_open("userdata", NVS_READWRITE, &nvs));
        ESP_ERROR_CHECK(nvs_set_i16(nvs, "version", 1));
    }

    channel_id = malloc(sizeof("door-") + sizeof(CONFIG_CLIENT_ID));
    sprintf(channel_id, "door-%s", CONFIG_CLIENT_ID);

    gpioInit();

    wifiCheckReset();
    {
        wifi_config_t wifi_config_stored;
        uint32_t len = sizeof(wifi_config_stored);
        esp_err_t ret = nvs_get_blob(nvs, "cfg.wifi", &wifi_config_stored, &len);

        if (ret != ESP_OK) {
            ESP_LOGI(TAG, "No WLAN configured. entering Smartconfig");
            systemStatus.isWlanSmartConfigRunning = 1;
            smartconfigBegin();

            ESP_LOGI(TAG, "Setting up BluFI for WLAN configuration");
            blufiInit();

            vTaskDelete(NULL);
            return;
        } else {
            esp_log_buffer_hex("Wifi config Data", &wifi_config_stored, len);
            wifi_init_sta(wifi_config_stored);
        }
    }

    ESP_LOGI(TAG, "Setting up BluFI for TOTP");
    blufiInit();

    ESP_LOGI(TAG, "WLAN Connected, Setting up NTP client");
    // Setting up NTP client
    ntpUpdate();
    ESP_LOGI(TAG, "NTP client up, UNIX Time %ld", time(NULL));

    ktotpInitSecret(NULL);
    ESP_LOGI(TAG, "Current TOTP key is %06u", ktotpGenerateToken(0));

    ESP_LOGI(TAG, "Setting up MQTT client");
    init_mqtt();

    ESP_LOGI(TAG, "Initialization completed");
    vTaskDelete(NULL);
    return;
}


void app_main(void)
{
    ESP_LOGD(TAG, "Base System Initialzing");
    ESP_LOGI(TAG, "========= Smart Gate Unlocker ===========\r\nWritten by Kenvix <i@kenvix.com> for AI+Mobile Internet Lab. All rights reserved.");
    ESP_LOGI(TAG, "Product Serial ID: 0x%llX (%lld)", keyData.serialId, keyData.serialId);
    ESP_LOGI(TAG, "Product TOTP key: %s", keyData.totpKey);
    systemStatus.isWlanConnected = 0;
    systemStatus.isNtpCreated = 0;
    systemStatus.isNtpFinished = 0;

    gpioAsync(gpioBeepOnce);
    xTaskCreate(init_all, "App init", 4096, NULL, 3, NULL);

    char ch;
    char cmdBuff[COMMAND_MAX_LEN + 1];
    int cmdLen = 0;

    while (true)
    {
        ch = getchar();
        if (ch == 0xFF || ch == 0x00) {
            vTaskDelay(80);
            continue;
        }
        else {
            gpioSetHigh(PIN_LED_SYS);
            if (ch == '\r')
                continue;

            if (ch == 0x04 || ch == 0x03) {
                ESP_LOGI(TAG, "Received CTRL+C OR EOF, drop cmd buffer.");
                cmdLen = 0;
                continue;
            }

            if (ch == '\n') {
                // Command collected
                cmdBuff[cmdLen] = 0;
                handleCommand(cmdBuff, cmdLen);
                cmdLen = 0;
            }
            else {
                // Collecting command chars
                if (cmdLen == COMMAND_MAX_LEN) {
                    ESP_LOGE(TAG, "Command buffer overflowed. Max size %d. DROPPED: %s", COMMAND_MAX_LEN, cmdBuff);
                    cmdLen = 0;

                    do {
                        ch = getchar();
                    } while (ch != 0xFF && ch != 0x00 && ch != '\n');
                    ESP_LOGD(TAG, "overflowed cmd dropped");
                    continue;
                }
                else {
                    cmdBuff[cmdLen] = ch;
                    cmdLen++;
                }
            }
            gpioSetLow(PIN_LED_SYS);
        }
    }
}
