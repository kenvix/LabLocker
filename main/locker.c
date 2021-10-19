#include "locker.h"


void openDoor()
{
    gpioSetHigh(OUTPUT_PIN_A);
    gpioSetHigh(OUTPUT_PIN_C);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpioSetLow(OUTPUT_PIN_A);
    gpioSetLow(OUTPUT_PIN_C);
}

void closeDoor()
{
    gpioSetHigh(OUTPUT_PIN_B);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpioSetLow(OUTPUT_PIN_B);
}

void initSuccess(){
    gpioSetHigh(OUTPUT_PIN_C);
    vTaskDelay(pdMS_TO_TICKS(50));
    gpioSetLow(OUTPUT_PIN_C);
    vTaskDelay(pdMS_TO_TICKS(50));
     gpioSetHigh(OUTPUT_PIN_C);
    vTaskDelay(pdMS_TO_TICKS(50));
    gpioSetLow(OUTPUT_PIN_C);
}
