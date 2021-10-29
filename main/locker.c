#include "locker.h"

void _gpioAsyncExec(void* param) {
    GPIOFunction f = (GPIOFunction) param;
    f();
    vTaskDelete(NULL);
}
                     
void gpioAsync(GPIOFunction f) {
    xTaskCreate(_gpioAsyncExec, "GPIO Async Task", 1024, f, 3, NULL);
}

void gpioBlinkNtp() {
    systemStatus.isNtpLedBlinking = 1;
    while (systemStatus.isNtpLedBlinking)
    {
        gpioSetHigh(PIN_LED_NTP);
        vTaskDelay(pdMS_TO_TICKS(300));

        gpioSetLow(PIN_LED_NTP);
        vTaskDelay(pdMS_TO_TICKS(300));
    }

    // NTP LED 共阳，低电平发光
    gpioSetHigh(PIN_LED_NTP);
}


void gpioBlinkWlan() {
    systemStatus.isWlanLedBlinking = 1;
    while (systemStatus.isWlanLedBlinking)
    {
        gpioSetHigh(PIN_LED_WLAN);
        vTaskDelay(pdMS_TO_TICKS(300));

        gpioSetLow(PIN_LED_WLAN);
        vTaskDelay(pdMS_TO_TICKS(300));
    }

    // NTP LED 共阳，低电平发光
    gpioSetHigh(PIN_LED_WLAN);
}


void gpioBlinkMqtt() {
    systemStatus.isMqttLedBlinking = 1;
    while (systemStatus.isMqttLedBlinking)
    {
        gpioSetHigh(PIN_LED_MQTT);
        vTaskDelay(pdMS_TO_TICKS(300));

        gpioSetLow(PIN_LED_MQTT);
        vTaskDelay(pdMS_TO_TICKS(300));
    }

    // NTP LED 共阳，低电平发光
    gpioSetHigh(PIN_LED_MQTT);
}

void gpioInit()
{
    gpio_set_direction(PIN_DOOR_OPEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_DOOR_CLOSE, GPIO_MODE_OUTPUT);

    gpio_set_direction(PIN_LED_MQTT, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_LED_NTP, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_LED_WLAN, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_BEEP, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_WLAN_RESET, GPIO_MODE_INPUT)

    gpioSetHigh(PIN_LED_MQTT);

    // NTP LED 共阳，低电平发光
    gpioSetHigh(PIN_LED_NTP);

    gpioSetHigh(PIN_LED_WLAN);
}

void gpioDoorOpen()
{
    gpioSetHigh(PIN_DOOR_OPEN);
    vTaskDelay(pdMS_TO_TICKS(DOOR_ROLLTATE_DELAY));
    gpioSetLow(PIN_DOOR_OPEN);
}

void gpioDoorClose()
{
    gpioSetHigh(PIN_DOOR_CLOSE);
    vTaskDelay(pdMS_TO_TICKS(DOOR_ROLLTATE_DELAY));
    gpioSetLow(PIN_DOOR_CLOSE);
}

void gpioDoorUnlock() {
    gpioDoorOpen();
    vTaskDelay(pdMS_TO_TICKS(DOOR_WAIT_USER_DELAY));
    gpioDoorClose();
}

void gpioBeepOnce() {
    gpioSetHigh(PIN_BEEP);
    vTaskDelay(pdMS_TO_TICKS(400));
    gpioSetLow(PIN_BEEP);
}

void initSuccess(){
    /*
    gpioSetHigh(OUTPUT_PIN_C);
    vTaskDelay(pdMS_TO_TICKS(50));
    gpioSetLow(OUTPUT_PIN_C);
    vTaskDelay(pdMS_TO_TICKS(50));
     gpioSetHigh(OUTPUT_PIN_C);
    vTaskDelay(pdMS_TO_TICKS(50));
    gpioSetLow(OUTPUT_PIN_C);
    */
}
