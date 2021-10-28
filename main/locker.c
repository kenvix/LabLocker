#include "locker.h"

void _gpioAsyncExec(void* param) {
    GPIOFunction f = (GPIOFunction) param;
    f();
    vTaskDelete(NULL);
}
                     
void gpioAsync(GPIOFunction f) {
    xTaskCreate(_gpioAsyncExec, "GPIO Async Task", 1024, f, 3, NULL);
}

void gpioInit()
{
    gpio_set_direction(PIN_DOOR_OPEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_DOOR_CLOSE, GPIO_MODE_OUTPUT);
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
