#include "locker.h"

void high(gpio_num_t pin)
{
    gpio_set_level(pin, 1);
}

void low(gpio_num_t pin)
{
    gpio_set_level(pin, 0);
}

void openDoor()
{
    high(OUTPUT_PIN_A);
    high(OUTPUT_PIN_C);
    vTaskDelay(pdMS_TO_TICKS(500));
    low(OUTPUT_PIN_A);
    low(OUTPUT_PIN_C);
}

void closeDoor()
{
    high(OUTPUT_PIN_B);
    vTaskDelay(pdMS_TO_TICKS(500));
    low(OUTPUT_PIN_B);
}

void initSuccess(){
    high(OUTPUT_PIN_C);
    vTaskDelay(pdMS_TO_TICKS(50));
    low(OUTPUT_PIN_C);
    vTaskDelay(pdMS_TO_TICKS(50));
     high(OUTPUT_PIN_C);
    vTaskDelay(pdMS_TO_TICKS(50));
    low(OUTPUT_PIN_C);
}
